#include <inttypes>

namespace i8080 {
	using Byte = uint8_t;
	using Word = uint16_t;
	
	struct cpuState;
}

typedef struct i8080::cpuState {
	//Registers
	Byte W, Z; //Temp Registers
	Byte B, C; //B Pair
	Byte D, E; //D Pair
	Byte H, L; //H Pair (Indirect Address)
	Byte A; //Accumulator

	//Combined Flag Register
	Byte flags; //Sign Zero - Auxillary Carry - Parity - Carry


};