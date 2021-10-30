#include "i8080.h"

using namespace i8080;

// All this at the top here is to avoid doing some comedically long switch statement when handling instructions.
// Believe me, another ~500-line switch statement is not something I want to do again...

bool arraySearch(Byte opcode, Byte insArray[]) {
	for (int i = 0; i <= sizeof(insArray); i++) {
		if (insArray[i] == opcode) {
			return true;
		}
	}
	return false;
}

Byte ACI[] = { 0xCE };
Byte ADC[] = { 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F };
Byte ADD[] = { 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87 };