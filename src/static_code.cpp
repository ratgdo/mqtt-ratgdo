#include "common.h"
#include "static_code.h"

void readStaticCode(byte rxSP1StaticCode[SECPLUS1_CODE_LEN], uint8_t &door, uint8_t &light, uint8_t &lock){
	static uint8_t prevDoor;
	
	uint8_t obs = 0; // experiement to figure out what key 0x39 is for
	uint8_t key = 0;
	uint8_t val = 0;

	key = rxSP1StaticCode[0];
	val = rxSP1StaticCode[1];

	printStaticCode(rxSP1StaticCode);
	Serial.print(" ");

	if(key == 0x31 && val == 0x31){
		door = 6; // stop from entering emulation after power outage when 889lm is booting up
	}

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

		// sec+1 doors sometimes report wrong door status
		// require two sequential matching door states 
		if(prevDoor != val){
			prevDoor = val;
			return;
		}

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

	// light & lock
	if(key == 0x3A){
		light = bitRead(val,2);
		lock =  !bitRead(val,3);
	}

	// obstruction?
	if(key == 0x39){
		obs = val;
	}
}

void getStaticCode(const char *command){
	if(strcmp(command,"door") == 0){
		txSP1StaticCode[0] = 0x30;
		txSP1StaticCode[1] = 0x31;
		txSP1StaticCode[2] = 0x31;
		txSP1StaticCode[3] = 0xFE;
	}else if(strcmp(command,"light") == 0){
		txSP1StaticCode[0] = 0x32;
		txSP1StaticCode[1] = 0x33;
		txSP1StaticCode[2] = 0x33;
		txSP1StaticCode[3] = 0xFE;
	}else if(strcmp(command,"lock") == 0){
		txSP1StaticCode[0] = 0x34;
		txSP1StaticCode[1] = 0x35;
		txSP1StaticCode[2] = 0x35;
		txSP1StaticCode[3] = 0xFE;
	}

	return;
}

void printStaticCode(byte code[SECPLUS1_CODE_LEN]){
	for(int i = 0; i < SECPLUS1_CODE_LEN; i++){
		if(code[i] <= 0x0f) Serial.print("0");
		Serial.print(code[i],HEX);
	}
}