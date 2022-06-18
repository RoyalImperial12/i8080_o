#pragma once

#include "processor/processor.h"

uint32_t memOffset;

#include <string>
#include <cstdio>
#include <stdio.h>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>

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

	i8080::Byte* buffer = &state.mem[memOffset];

	fread(buffer, 2, fsize, f);

	fclose(f);

	memOffset += fsize;
}

void initMem() {
	state.sign = state.zero = state.parity = state.carry = state.auxillaryCarry = 0;

	memset(&state.A, 0x0, sizeof(i8080::Byte)); memset(&state.B, 0x0, sizeof(i8080::regPair)); memset(&state.D, 0x0, sizeof(i8080::regPair)); memset(&state.H, 0x0, sizeof(i8080::regPair)); memset(&state.tmp, 0x0, sizeof(i8080::regPair));

	state.PC = state.SP = 0x0;

	state.interruptEnable = false;
}

int main() {
	initMem();

	std::thread([]() { loadMem("rom/invaders.h");
	loadMem("rom/invaders.g");
	loadMem("rom/invaders.f");
	loadMem("rom/invaders.e"); }).join();

	printf("Parsing data...\n");
	std::thread(opParseMem).join();

	boost::posix_time::ptime lastTimer;

	printf("Executing...\n");

	int cycles = 0;

	std::thread t([cycles]()->void {});

	while (true) {
		//uint32_t now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();

		if (lastTimer.is_not_a_date_time()) {
			lastTimer = now;
		}

		cycles = 0;

		while (cycles < CLOCKSPEED) {
			cycles += handleIns();
		}

		//now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		now = boost::posix_time::microsec_clock::universal_time();

		boost::posix_time::time_duration run = now - lastTimer;
		uint64_t remTime = SECONDMS - run.total_microseconds();

		if (remTime > SECONDMS) {
			remTime = 0;
		}

		printf("%u\n", run.total_microseconds());
		printf("%u\n\n", remTime);

		lastTimer = now;

		boost::this_thread::sleep_for(boost::chrono::microseconds(remTime));
	}

	delete& state;

	return 1;
}