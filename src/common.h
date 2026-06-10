#include <Arduino.h>

#define SECPLUS2_CODE_LEN 19 // the length of each command sent to the door.
extern byte txSP2RollingCode[SECPLUS2_CODE_LEN];
extern byte rxSP2RollingCode[SECPLUS2_CODE_LEN];
extern uint32_t rollingCodeCounter;
extern uint32_t idCode;
extern String controlProtocol;

#define SECPLUS1_CODE_LEN 2
extern byte txSP1StaticCode[4];
extern byte rxSP1StaticCode[SECPLUS1_CODE_LEN];