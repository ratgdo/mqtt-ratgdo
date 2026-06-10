#include "common.h"
#include "rolling_code.h"
#include "Helpers.h"

// Security+ 2.0 rolling codes are 28 bits wide. encode_wireline() fails for any
// counter >= 2^28, so the counter must always be wrapped into the 28 bit range.
static const uint32_t ROLLING_CODE_MASK = 0xFFFFFFF;

void readCounterFromFlash(const char *type, uint32_t &counter){

	File file = LittleFS.open(type, "r");

	//Check if the file exists
	if(!file){
		Serial.print(type);
		Serial.println(" doesn't exist. creating...");

		writeCounterToFlash(type,counter);
		return;
	}

    // Wrap the stored value into the valid 28 bit range so a corrupted or
    // previously overflowed value in flash can never overflow the encoder again
    counter = static_cast<uint32_t>(file.parseInt()) & ROLLING_CODE_MASK;

	//Close the file
	file.close();
}

void writeCounterToFlash(const char *type, uint32_t &counter){
	//Open the file 
	File file = LittleFS.open(type, "w");
	
	//Write to the file
	file.print(counter);
	delay(1);
	//Close the file
	file.close();
	
	Serial.print(type);
	Serial.println(" write successful");
}

void readRollingCode(byte rxSP2RollingCode[SECPLUS2_CODE_LEN], uint8_t &door, uint8_t &light, uint8_t &lock, uint8_t &motion, uint8_t &obstruction){
	uint32_t rolling = 0;
	uint64_t fixed = 0;
	uint32_t data = 0;

	uint16_t cmd = 0;
	uint8_t nibble = 0;
	uint8_t byte1 = 0;
	uint8_t byte2 = 0;

	decode_wireline(rxSP2RollingCode, &rolling, &fixed, &data);

	cmd = ((fixed >> 24) & 0xf00) | (data & 0xff);

	nibble = (data >> 8) & 0xf;
	byte1 = (data >> 16) & 0xff;
	byte2 = (data >> 24) & 0xff;

	printRollingCode(rxSP2RollingCode);

	if(cmd == 0x81){
		door = nibble;
		light = (byte2 >> 1) & 1;
		lock = byte2 & 1;
		motion = 0; // when the status message is read, reset motion state to 0|clear
		// obstruction = (byte1 >> 6) & 1; // unreliable due to the time it takes to register an obstruction

		Serial.print(" | STATUS:");
		Serial.print(" door:");
		Serial.print(nibble);
		Serial.print(" light:");
		Serial.print((byte2 >> 1) & 1);
		Serial.print(" lock:");
		Serial.print((byte2 & 1));
		Serial.print(" obs:");
		Serial.print((byte1 >> 6) & 1);

	}else if(cmd == 0x281){
		light ^= 1; // toggle bit

		Serial.print(" | LIGHT:");
		Serial.print(light);
	}else if(cmd == 0x84){
	}else if(cmd == 0x285){
		motion = 1; // toggle bit
		Serial.print(" | MOTION:");
		Serial.print(motion);
	}

	Serial.println("");
}

void getRollingCode(const char *command){
    // Defensively wrap the counter before encoding; encode_wireline rejects values >= 2^28
    rollingCodeCounter &= ROLLING_CODE_MASK;

	Serial.print("rolling code for ");
	Serial.print(idCode, HEX);
	Serial.print(" ");
	Serial.print(rollingCodeCounter);
	Serial.print("|");
	Serial.print(command);
	Serial.print(" : ");

	uint64_t id = idCode;
	uint64_t fixed = 0;
	uint32_t data = 0;

	if(strcmp(command,"reboot1") == 0){
		fixed = 0x400000000;
		data = 0x0000008b;
	}else if(strcmp(command,"reboot2") == 0){
		fixed = 0;
		data = 0x00000080;
	}else if(strcmp(command,"reboot3") == 0){
		fixed = 0;
		data = 0x000000a0;
	}else if(strcmp(command,"reboot4") == 0){
		fixed = 0;
		data = 0x00000080;
	}else if(strcmp(command,"reboot5") == 0){
		fixed = 0x300000000;
		data = 0x00000092;
	}else if(strcmp(command,"reboot6") == 0){
		fixed = 0x300000000;
		data = 0x00000092;
	}else if(strcmp(command,"door1") == 0){
		fixed = 0x200000000;
		data = 0x01010280;
	}else if(strcmp(command,"door2") == 0){
		fixed = 0x200000000;
		data = 0x01000280;
	}else if(strcmp(command,"light") == 0){
		fixed = 0x200000000;
		data = 0x00000281;
	}else if(strcmp(command,"lock") == 0){
		fixed = 0x0100000000;
		data = 0x0000028c;
	}else{
		Serial.println("ERROR: Invalid command");
		return;
	}

	fixed = fixed | id;

    // Abort if the encoder rejects the inputs so a stale/garbage packet is never transmitted
    const int8_t encodeResult = encode_wireline(rollingCodeCounter, fixed, data, txSP2RollingCode);
    if(encodeResult != 0){
        Serial.println("ERROR: rolling code encode failed");
        return;
    }

	printRollingCode(txSP2RollingCode);
	Serial.println("");

	if(strcmp(command,"door1") != 0){ // door2 is created with same counter and should always be called after door1
        // Wrap at 2^28 so the counter never overflows the Security+ 2.0 rolling code space
        rollingCodeCounter = (rollingCodeCounter + 1) & ROLLING_CODE_MASK;
	}
	return;
}

void printRollingCode(byte code[SECPLUS2_CODE_LEN]){
	for(int i = 0; i < SECPLUS2_CODE_LEN; i++){
		if(code[i] <= 0x0f) Serial.print("0");
		Serial.print(code[i],HEX);
	}
}