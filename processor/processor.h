#include "..\i8080namespace.h"

typedef struct i8080::i8080State {
	//Registers
	Byte W, Z; //Temp Registers
	Byte B, C; //B Pair
	Byte D, E; //D Pair
	Byte H, L; //H Pair (Indirect Address)
	Byte A; //Accumulator

	Word SP; //Stack Pointer
	Word PC; //Program Counter

	Byte sign : 1;
	Byte zero : 1;
	Byte parity : 1;
	Byte auxillaryCarry : 1;
	Byte carry : 1;

	bool interruptEnable;

	Byte port[256];
	Byte mem[65536];
} i8080State;