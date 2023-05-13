#pragma once
#include "..\i8080namespace.h"

typedef struct i8080::i8080State {
	//Registers
	regPair tmp; //Temp Registers
	regPair B; //B Pair
	regPair D; //D Pair
	regPair H; //H Pair (Indirect Address)
	Byte A; //Accumulator

	Word SP; //Stack Pointer
	Word PC; //Program Counter

	Byte sign : 1;
	Byte zero : 1;
	Byte parity : 1;
	Byte auxillaryCarry : 1;
	Byte carry : 1;

	bool interruptEnable;

	Byte port[255];

	Byte mem[65535];
	Byte insMem[65535];
} i8080State;

void opParseMem();

static i8080::i8080State&& state = {};

void deleteProc();

int handleIns();