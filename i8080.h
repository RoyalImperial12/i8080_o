#include <inttypes>

namespace i8080 {
	using Byte = uint8_t;
	using Word = uint16_t;
	
	struct i8080State;
}

typedef struct i8080::i8080State {
	//Registers
	Byte W, Z; //Temp Registers
	Byte B, C; //B Pair
	Byte D, E; //D Pair
	Byte H, L; //H Pair (Indirect Address)
	Byte A; //Accumulator

	Word SP; //Stack Pointer
	Word PC; //Program Counter

	//Combined Flag Register
	Byte flags; //Sign Zero - Auxillary Carry - Parity - Carry

	Byte& port[256];
	Byte& mem[65536];
};