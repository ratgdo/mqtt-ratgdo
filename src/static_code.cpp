#include "common.h"
#include "static_code.h"

void readStaticCode(byte rxSP1StaticCode[SECPLUS1_CODE_LEN], uint8_t &door, uint8_t &light){
	uint8_t obs = 0; // experiement to figure out what key 0x39 is for
	uint8_t key = 0;
	uint8_t val = 0;

	key = rxSP1StaticCode[0];
	val = rxSP1StaticCode[1];

	printStaticCode(rxSP1StaticCode);
	Serial.print(" ");

	if(key == 0xFF || val == 0xFF){
		return;
	}

	// door state
	if(key == 0x38){
		Serial.print(" | door state: ");
		Serial.print(val,HEX);
		Serial.print(" ");
		Serial.println(val,BIN);

		val = (val & 0x7);
		// 000 0x0 stopped
		// 001 0x1 opening
		// 010 0x2 open
		// 100 0x4 closing
		// 101 0x5 closed
		// 110 0x6 stopped

		if(val == 0x2){
			// door open
			door = 1;
		}else if(val == 0x5){
			// door closed
			door = 2;
		}else if(val == 0x0 || val == 0x6){
			// door stopped
			door = 3;
		}else if(val == 0x1){
			// door opening
			door = 4;
		}else if(val == 0x4){
			// door closing
			door = 5;
		}else{
			// unknown
			door = 0;
		}
	}

	// light
	if(key == 0x3A){
		light = bitRead(val,2);
	}

	// obstruction?
	if(key == 0x39){
		obs = val;
	}

	// Serial.print(" | STATUS:");
	// Serial.print(" door:");
	// Serial.print(door);
	// Serial.print(" light:");
	// Serial.print(light);
	// Serial.print(" obs?:");
	// Serial.print(obs);

	// Serial.println("");
}

void getStaticCode(const char *command){
	if(strcmp(command,"door1") == 0){
		txSP1StaticCode[0] = 0x30;
	}else if(strcmp(command,"door2") == 0){
		txSP1StaticCode[0] = 0x31;
	}else if(strcmp(command,"light") == 0){
		txSP1StaticCode[0] = 0x32;
	}

	return;
}

void printStaticCode(byte code[SECPLUS1_CODE_LEN]){
	for(int i = 0; i < SECPLUS1_CODE_LEN; i++){
		if(code[i] <= 0x0f) Serial.print("0");
		Serial.print(code[i],HEX);
	}
}