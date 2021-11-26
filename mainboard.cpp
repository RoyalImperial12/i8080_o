#include "i8080namespace.h"

#include "processor/processor.h"

i8080State* procState = new i8080State;
uint32_t memOffset;

#include <string>
#include <chrono>
#include <thread>
#include <cstdio>
#include <stdio.h>

#define CLOCKSPEED 2000000
#define SECONDMS 1000000

//Need to figure out a better and automatic way of doing this...

void loadMem(std::string fileName) {
	const char* cfileName = fileName.c_str();

	FILE* f = fopen(cfileName, "r");

	if (f == NULL) {
		printf("ERROR: failed to open %s.\n", cfileName);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	i8080::Byte* buffer = &procState->mem[memOffset];

	fread(buffer, 2, fsize, f);

	fclose(f);

	memOffset += fsize;
}

void initMem() {
	procState->sign = procState->zero = procState->parity = procState->carry = procState->auxillaryCarry = 0;

	procState->A = procState->B = procState->C = procState->D = procState->E = procState->H = procState->L = 0x0;

	procState->PC = procState->SP = 0x0;

	procState->interruptEnable = false;
}

void main() {
	initMem();

	//loadMem();

	uint64_t lastTimer = 0;

	while (true) {
		uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		if (lastTimer == 0) {
			lastTimer = now;
		}

		int cycles = 0;

		while (cycles < CLOCKSPEED) {
			cycles += handleIns();
		}

		now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		uint64_t run = now - lastTimer;
		uint64_t remTime = SECONDMS - run;

		printf("Took %u\n", run);
		printf("Sleep for %u\n", remTime);

		lastTimer = now;

		std::this_thread::sleep_for(std::chrono::microseconds(remTime));
	}

	procClear();

	delete procState;
}