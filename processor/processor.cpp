#pragma once

#include "processor.h"
#include <iostream>
#include <map>

using namespace i8080;

i8080State* state = new i8080State;

// Cycle Switches
class cycleSwitches {
public:
	int Rccc = 5; //R Condition
	int Cccc = 11; //C Condition
} cycleSwitches;

// CPU Cycles
int CPUCycles[] = {
	4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x00...F
	4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x10...F
	4, 10, 16, 5, 5, 5, 7, 4, 4, 10, 16, 5, 5, 5, 7, 4, //0x20...F
	4, 10, 13, 5, 10, 10, 10, 4, 4, 10, 13, 5, 5, 5, 7, 4, //0x40...F

	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, //0x40...F
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, //0x50...F
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, //0x60...F
	7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 5, //0x70...F

	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0x80...F
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0x90...F
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0xA0...F
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0xB0...F

	cycleSwitches.Rccc, 10, 10, 10, cycleSwitches.Cccc, 11, 7, 11, cycleSwitches.Rccc, 10, 10, 10, cycleSwitches.Cccc, 17, 7, 11, //0xC0...F
	cycleSwitches.Rccc, 10, 10, 10, cycleSwitches.Cccc, 11, 7, 11, cycleSwitches.Rccc, 10, 10, 10, cycleSwitches.Cccc, 17, 7, 11, //0xD0...F
	cycleSwitches.Rccc, 10, 10, 18, cycleSwitches.Cccc, 11, 7, 11, cycleSwitches.Rccc, 5, 10, 4, cycleSwitches.Cccc, 17, 7, 11, //0XE0...F
	cycleSwitches.Rccc, 10, 10, 4, cycleSwitches.Cccc, 11, 7, 11, cycleSwitches.Rccc, 5, 10, 4, cycleSwitches.Cccc, 17, 7, 11 //0xF0...F
};

/* The code that follows is to avoid me doing some comically long switch statement when handling instructions.
Believe me, another ~500-line switch statement which seperates each opcode into an indivdual case is not something I want to do again...
Especially when the functions that actually emulates the instruction will identify the appropriate registers anyway. */

int opMnemonic[] = { 'ACI', 'ADC', 'ADD', 'ADI', 'ANA', 'ANI', 
					  'CALL', 'CC', 'CM', 'CMA', 'CMC', 'CMP', 'CNC', 'CNZ', 'CP', 'CPE', 'CPI', 'CPO', 'CZ', 
					  'DAA', 'DAD', 'DCR', 'DCX', 'DI',
					  'EI', 
					  'HLT',
					  'IN', 'INR', 'INX', 
					  'JC', 'JM', 'JMP', 'JNC', 'JNZ', 'JP', 'JPE', 'JPO', 'JZ', 
					  'LDA', 'LDAX', 'LHLD', 'LXI',
					  'MOV', 'MVI', 
					  'NOP',
					  'ORA', 'ORI',
					  'OUT',
					  'PCHL', 'POP', 'PUSH',
					  'RAL', 'RAR', 'RC', 'RET', 'RLC', 'RM', 'RNC', 'RNZ', 'RP', 'RPE', 'RPO', 'RRC', 'RST', 'RZ',
					  'SBB', 'SBI', 'SHLD', 'SPHL', 'STA', 'STAX', 'STC', 'SUB', 'SUI',
					  'XCHG', 'XRA', 'XRI', 'XTHL' };

Byte opACI[] = { 0xCE };
Byte opADC[] = { 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F };
Byte opADD[] = { 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87 };
Byte opADI[] = { 0xC6 };
Byte opANA[] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7 };
Byte opANI[] = { 0xE6 };
Byte opCALL[] = { 0xCD, 0xDD, 0xED, 0xFD };
Byte opCC[] = { 0xDC };
Byte opCM[] = { 0xFC };
Byte opCMA[] = { 0x2F };
Byte opCMC[] = { 0x3F };
Byte opCMP[] = { 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF };
Byte opCNC[] = { 0xD4 };
Byte opCNZ[] = { 0xC4 };
Byte opCP[] = { 0xF4 };
Byte opCPE[] = { 0xEC };
Byte opCPI[] = { 0xFE };
Byte opCPO[] = { 0xE4 };
Byte opCZ[] = { 0xCC };
Byte opDAA[] = { 0x27 };
Byte opDAD[] = { 0x09, 0x19, 0x29, 0x39 };
Byte opDCR[] = { 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x35, 0x3D };
Byte opDCX[] = { 0x0B, 0x1B, 0x2B, 0x3B };
Byte opDI[] = { 0xF3 };
Byte opEI[] = { 0xFB };
Byte opHLT[] = { 0x76 };
Byte opIN[] = { 0xDB };
Byte opINR[] = { 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C };
Byte opINX[] = { 0x03, 0x13, 0x23, 0x33 };
Byte opJC[] = { 0xDA };
Byte opJM[] = { 0xFA };
Byte opJMP[] = { 0xC3, 0xCB };
Byte opJNC[] = { 0xD2 };
Byte opJNZ[] = { 0xC2 };
Byte opJP[] = { 0xF2 };
Byte opJPE[] = { 0xEA };
Byte opJPO[] = { 0xE2 };
Byte opJZ[] = { 0xCA };
Byte opLDA[] = { 0x3A };
Byte opLDAX[] = { 0x0A, 0x1A };
Byte opLHLD[] = { 0x2A };
Byte opLXI[] = { 0x01, 0x11, 0x21, 0x31 };
Byte opMOV[] = { 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
			   0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 
			   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 
			   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F };
Byte opMVI[] = { 0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x36, 0x3E };
Byte opNOP[] = { 0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38 };
Byte opORA[] = { 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7 };
Byte opORI[] = { 0xF6 };
Byte opOUT[] = { 0xD3 };
Byte opPCHL[] = { 0xE9 };
Byte opPOP[] = { 0xC1, 0xD1, 0xE1, 0xF1 };
Byte opPUSH[] = { 0xC5, 0xD5, 0xE5, 0xF5 };
Byte opRAL[] = { 0x17 };
Byte opRAR[] = { 0x1F };
Byte opRC[] = { 0xD8 };
Byte opRET[] = { 0xC9, 0xD9 };
Byte opRLC[] = { 0x07 };
Byte opRM[] = { 0xF8 };
Byte opRNC[] = { 0xD0 };
Byte opRNZ[] = { 0xC0 };
Byte opRP[] = { 0xF0 };
Byte opRPE[] = { 0xE8 };
Byte opRPO[] = { 0xE0 };
Byte opRRC[] = { 0x0F };
Byte opRST[] = { 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF };
Byte opRZ[] = { 0xC8 };
Byte opSBB[] = { 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F };
Byte opSBI[] = { 0xDE };
Byte opSHLD[] = { 0x22 };
Byte opSPHL[] = { 0xF9 };
Byte opSTA[] = { 0x32 };
Byte opSTAX[] = { 0x02, 0x12 };
Byte opSTC[] = { 0x37 };
Byte opSUB[] = { 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97 };
Byte opSUI[] = { 0xD6 };
Byte opXCHG[] = { 0xEB };
Byte opXRA[] = { 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF };
Byte opXRI[] = { 0xEE };
Byte opXTHL[] = { 0xE3 };

std::map<int, Byte*> op{ {'ACI', opACI}, {'ADC', opADC}, {'ADD', opADD}, {'ADI', opADI}, {'ANA', opANA}, {'ANI', opANI},
						  {'CALL', opCALL}, {'CC', opCC}, {'CM', opCM}, {'CMA', opCMA}, {'CMC', opCMC}, {'CMP', opCMP}, {'CNC', opCNC}, {'CNZ', opCNZ}, {'CP', opCP}, {'CPE', opCPE}, {'CPI', opCPI}, {'CPO', opCPO}, {'CZ', opCZ},
						  {'DAA', opDAA}, {'DAD', opDAD}, {'DCR', opDCR}, {'DCX', opDCX}, {'DI', opDI},
						  {'EI', opEI},
						  {'HLT', opHLT},
						  {'IN', opIN}, {'INR', opINR}, {'INX', opINX},
						  {'JC', opJC}, {'JM', opJM}, {'JMP', opJMP}, {'JNC', opJNC}, {'JNZ', opJNZ}, {'JP', opJP}, {'JPE', opJPE}, {'JPO', opJPO}, {'JZ', opJZ},
						  {'LDA', opLDA}, {'LDAX', opLDAX}, {'LHLD', opLHLD}, {'LXI', opLXI},
						  {'MOV', opMOV}, {'MVI', opMVI},
						  {'NOP', opNOP},
						  {'ORA', opORA}, {'ORI', opORI},
						  {'OUT', opOUT},
						  {'PCHL', opPCHL}, {'POP', opPOP}, {'PUSH', opPUSH},
						  {'RAL', opRAL}, {'RAR', opRAR}, {'RC', opRC}, {'RET', opRET}, {'RLC', opRLC}, {'RM', opRM}, {'RNC', opRNC}, {'RNZ', opRNZ}, {'RP', opRP}, {'RPE', opRPE}, {'RPO', opRPO}, {'RRC', opRRC}, {'RST', opRST}, {'RZ', opRZ},
						  {'SBB', opSBB}, {'SBI', opSBI}, {'SHLD', opSHLD}, {'SPHL', opSPHL}, {'STA', opSTA}, {'STAX', opSTAX}, {'STC', opSTC}, {'SUB', opSUB}, {'SUI', opSUI},
						  {'XCHG', opXCHG}, {'XRA', opXRA}, {'XRI', opXRI}, {'XTHL', opXTHL} };

int opArraySearch(Byte opcode) {
	int ins = 0;

	for (int x = 0; x < sizeof(opMnemonic); x++) {
		for (int y = 0; y < sizeof(op[opMnemonic[x]]); y++) {
			if (op[opMnemonic[x]][y] == opcode) {
				ins = opMnemonic[x];
				return ins;
			}
		}
	}

	return ins;
}

void determineFlag(int sign, int zero, int auxillaryCarry, int parity, int carry, Byte reg) {
	//Sign
	if (sign == 1) {
		state->sign = (reg >> 7);
	}
	//Zero
	if (zero == 1) {
		state->zero = (reg == 0);
	}
	//Auxillary Carry
	if (auxillaryCarry == 1) {
		state->auxillaryCarry = (reg > 0x09);
	}
	//Parity
	if (parity == 1) {
		state->parity = (reg % 2 == 0);
	}
	//Carry
	if (carry == 1) {
		state->carry = (reg > 0xFF);
	}

	return;
}

//ASM Commands BEGIN
void ACI() { //2 Bytes, 7 Cycles, SZAcPC
	Byte value = state->mem[state->PC];
	state->PC++;

	state->A += value + state->C;
	determineFlag(1, 1, 1, 1, 1, state->A);
}

void ADC(Byte opcode) { //1 Byte, 4 Cycles (7 for M), SZAcPC
	Byte srcReg = (opcode << 5) >> 5;

	Byte src = 0x0;
	Word srcAddress = 0x0;
	switch (srcReg) {
	case 0b111: //A
		src = state->A;
		break;
	case 0b000: //B
		src = state->B;
		break;
	case 0b001: //C
		src = state->C;
		break;
	case 0b010: //D
		src = state->D;
		break;
	case 0b011: //E
		src = state->E;
		break;
	case 0b100: //H
		src = state->H;
		break;
	case 0b101: //L
		src = state->L;
		break;
	case 0b110: //M
		srcAddress = (state->H << 8) | state->L;
		break;
	}

	if (srcReg == 0b110)
		state->A += state->mem[srcAddress];
	else
		state->A += src;

	state->A += state->carry;

	determineFlag(1, 1, 1, 1, 1, state->A);
}

void ADD(Byte opcode) { //1 Byte, 4 Cycles (7 for M), SZAcPC
	Byte srcReg = (opcode << 5) >> 5;

	Byte src = 0x0;
	Word srcAddress = 0x0;
	switch (srcReg) {
	case 0b111: //A
		src = state->A;
		break;
	case 0b000: //B
		src = state->B;
		break;
	case 0b001: //C
		src = state->C;
		break;
	case 0b010: //D
		src = state->D;
		break;
	case 0b011: //E
		src = state->E;
		break;
	case 0b100: //H
		src = state->H;
		break;
	case 0b101: //L
		src = state->L;
		break;
	case 0b110: //M
		srcAddress = (state->H << 8) | state->L;
		break;
	}

	if (srcReg == 0b110)
		state->A += state->mem[srcAddress];
	else
		state->A += src;

	determineFlag(1, 1, 1, 1, 1, state->A);
}

void ADI() { //2 Bytes, 7 Cycles, SZeAcPCy.
	Byte value = state->PC;
	state->PC++;

	state->A += value;
	determineFlag(1, 1, 1, 1, 1, state->A);
}

void ANA() { //2 Bytes, 7 Cycles, SZAcPC.
	Byte value = state->PC;
	state->PC++;

	state->A += value;
	determineFlag(1, 1, 1, 1, 1, state->A);
}

void ANI() { //2 Bytes, 7 Cycles, SZAcPC.
	Byte value = state->mem[state->PC];
	state->PC++;

	state->A &= value;
	determineFlag(1, 1, 1, 1, 1, state->A);
}

void CALL() { //3 Bytes, 17 Cycles.
	state->mem[state->SP - 1] = (state->PC >> 8);
	state->mem[state->SP - 2] = (state->PC << 8) >> 8;
	state->SP -= 2;

	Word address = (state->mem[state->PC + 1] << 8) | state->mem[state->PC];
	state->PC = address;
}

void CMA() { //1 Byte, 4 Cycles.
	state->A = ~state->A;
}

void CMC() { //1 Byte, 4 Cycles, C.
	state->carry = ~state->carry;
}

void CMP(Byte opcode) { //1 Byte, 4 Cycles (7 for M), SZeAcPCy.
	Byte srcReg = (opcode << 5) >> 5;

	Byte src = 0x0;
	Word srcAddress = 0x0;
	switch (srcReg) {
	case 0b111: //A
		src = state->A;
		break;
	case 0b000: //B
		src = state->B;
		break;
	case 0b001: //C
		src = state->C;
		break;
	case 0b010: //D
		src = state->D;
		break;
	case 0b011: //E
		src = state->E;
		break;
	case 0b100: //H
		src = state->H;
		break;
	case 0b101: //L
		src = state->L;
		break;
	case 0b110: //M
		srcAddress = (state->H << 8) | state->L;
		break;
	}

	Byte res = 0x0;

	if (srcReg == 0b110)
		res = state->A - state->mem[srcAddress];
	else
		res = state->A - src;

	determineFlag(1, 1, 0, 1, 0, res);
	state->carry = (state->A < res);
	state->auxillaryCarry = (state->A > 0x09);
}

void CPI() { //2 Bytes, 7 Cycles, SZeAcPCy.
	Byte res = state->A - (state->mem[state->PC]);
	state->PC++;

	determineFlag(1, 1, 0, 1, 0, res);
	state->carry = (state->A < res);
	state->auxillaryCarry = (state->A > 0x09);
}

void DAA() { //1 Byte, 4 Cycles, SZAcPC.
	Byte LSB = (state->A << 4) >> 4;
	if ((LSB > 9) || (state->auxillaryCarry == 1)) {
		Byte result = LSB + 0x6;
		state->A += 0x06;
		determineFlag(1, 1, 1, 1, 1, result);
	}

	Byte MSB = state->A >> 4;
	if ((MSB > 9) || (state->carry == 1)) {
		Byte result = MSB + 0x6;
		state->A += 0x06;
		determineFlag(1, 1, 1, 1, 1, result);
	}
}

void DAD(Byte opcode) { //1 Byte, 10 Cycles, C.
	Byte regPair = (opcode << 2) >> 6;

	Word regPairValue = 0;
	switch (regPair) {
	case 0b00: //B
		regPairValue = (state->B << 8) | state->C;
		break;
	case 0b01: //D
		regPairValue = (state->D << 8) | state->E;
		break;
	case 0b10: //H
		regPairValue = (state->H << 8) | state->L;
		break;
	case 0b11: //SP
		regPairValue = state->SP;
		break;
	}

	Word HL = (state->H << 8) | state->L;
	Word result = HL + regPairValue;

	state->carry = ((result & 0xFF000) > 0);

	state->H = (result & 0xFF00) >> 8;
	state->L = (result & 0xFF);
}

void DCR(Byte opcode) { //1 Byte, 5 Cycles (10 for M), SZAcP.
	Byte reg = (opcode << 2) >> 5;

	Word address = 0x0;

	switch (reg) {
	case 0b000:
		state->B--;
		determineFlag(1, 1, 1, 1, 0, state->B);
		break;
	case 0b001:
		state->C--;
		determineFlag(1, 1, 1, 1, 0, state->C);
		break;
	case 0b010:
		state->D--;
		determineFlag(1, 1, 1, 1, 0, state->D);
		break;
	case 0b011:
		state->E--;
		determineFlag(1, 1, 1, 1, 0, state->E);
		break;
	case 0b100:
		state->H--;
		determineFlag(1, 1, 1, 1, 0, state->H);
		break;
	case 0b101:
		state->L--;
		determineFlag(1, 1, 1, 1, 0, state->L);
		break;
	case 0b110:
		address = (state->H << 8) | state->L;
		state->mem[address]--;
		determineFlag(1, 1, 1, 1, 0, state->mem[address]);
		break;
	case 0b111:
		state->A--;
		determineFlag(1, 1, 1, 1, 0, state->A);
		break;
	}
}

void DCX(Byte opcode) { //1 Byte, 5 Cycles.
	Byte regPair = (opcode << 2) >> 6;

	switch (regPair) {
	case 0b00: //B
		state->B--;
		state->C--;
		break;
	case 0b01: //D
		state->D--;
		state->E--;
		break;
	case 0b10: //H
		state->H--;
		state->L--;
		break;
	case 0b11: //SP
		state->SP--;
		break;
	}
}

void INR(Byte opcode) { //1 Byte, 5 Cycles (10 for M), SZAcP.
	Byte reg = (opcode << 2) >> 5;

	Word address = 0x0;

	switch (reg) {
	case 0b000:
		state->B++;
		determineFlag(1, 1, 1, 1, 0, state->B);
		break;
	case 0b001:
		state->C++;
		determineFlag(1, 1, 1, 1, 0, state->C);
		break;
	case 0b010:
		state->D++;
		determineFlag(1, 1, 1, 1, 0, state->D);
		break;
	case 0b011:
		state->E++;
		determineFlag(1, 1, 1, 1, 0, state->E);
		break;
	case 0b100:
		state->H++;
		determineFlag(1, 1, 1, 1, 0, state->H);
		break;
	case 0b101:
		state->L++;
		determineFlag(1, 1, 1, 1, 0, state->L);
		break;
	case 0b110:
		address = (state->H << 8) | state->L;
		state->mem[address]++;
		determineFlag(1, 1, 1, 1, 0, state->mem[address]);
		break;
	case 0b111:
		state->A++;
		determineFlag(1, 1, 1, 1, 0, state->A);
		break;
	}
}

void INX(Byte opcode) { //1 Byte, 5 Cycles.
	Byte regPair = (opcode << 2) >> 6;

	switch (regPair) {
	case 0b00: //BC Pair
		state->B++;
		state->C++;
		break;
	case 0b01: //DE Pair
		state->D++;
		state->E++;
		break;
	case 0b10: //HL Pair
		state->H++;
		state->L++;
		break;
	case 0b11: //SP
		state->SP++;
		break;
	}
}

void JMP() { // Bytes, 10 Cycles.
	Word address = (state->mem[state->PC + 1] << 8) | state->mem[state->PC];
	state->PC += 2;

	state->PC = address;
}

void LDAX(Byte opcode) { //1 Byte, 7 Cycles.
	Byte regPair = (opcode << 2) >> 6;

	Word address = 0x0;
	switch (regPair) {
	case 0b00: //B
		address = (state->B << 8) | state->C;
		break;
	case 0b01: //D
		address = (state->D << 8) | state->E;
		break;
	}

	state->A = state->mem[address];
}

void LHLD() { //3 Byte, 16 Cycles.
	Word address = state->mem[state->PC] | (state->mem[state->PC++] << 8);
	state->PC++;

	state->L = state->mem[address];
	state->H = state->mem[address++];
}

void LXI(Byte opcode) { //3 Bytes, 10 Cycles.
	Byte regPair = (opcode << 2) >> 6;

	switch (regPair) {
	case 0b00: //BC Pair
		state->C = state->mem[state->PC], state->PC++;
		state->B = state->mem[state->PC], state->PC++;
		break;
	case 0b01: //DE Pair
		state->E = state->mem[state->PC], state->PC++;
		state->D = state->mem[state->PC], state->PC++;
		break;
	case 0b10: //HL Pair
		state->L = state->mem[state->PC], state->PC++;
		state->H = state->mem[state->PC], state->PC++;
		break;
	case 0b11: //SP
		state->SP = state->mem[state->PC] | (state->mem[state->PC++] << 8); state->PC++;
		break;
	}
}

void MOV(Byte opcode) { //1 Byte, 5 Cycles (7 if M involved).
	Byte dstBinary = (opcode << 2) >> 5;
	Byte srcBinary = (opcode << 5) >> 5;

	Byte src = 0x0;
	Word address = 0x0;

	switch (srcBinary) {
	case 0b111: //A
		src = state->A;
		break;
	case 0b000: //B
		src = state->B;
		break;
	case 0b001: //C
		src = state->C;
		break;
	case 0b010: //D
		src = state->D;
		break;
	case 0b011: //E
		src = state->E;
		break;
	case 0b100: //H
		src = state->H;
		break;
	case 0b101: //L
		src = state->L;
		break;
	case 0b110: //M
		address = (state->H << 8) | state->L;
		src = state->mem[address];
		break;
	}

	switch (dstBinary) {
	case 0b111: //A
		state->A = src;
		break;
	case 0b000: //B
		state->B = src;
		break;
	case 0b001: //C
		state->C = src;
		break;
	case 0b010: //D
		state->D = src;
		break;
	case 0b011: //E
		state->E = src;
		break;
	case 0b100: //H
		state->H = src;
		break;
	case 0b101: //L
		state->L = src;
		break;
	case 0b110: //M
		address = (state->H << 8) | state->L;
		state->mem[address] = src;
		break;
	}
}

void MVI(Byte opcode) { //2 Bytes, 7 Cycles (10 for M).
	Byte reg = (opcode << 2) >> 5;

	Byte data = state->mem[state->PC];
	state->PC++;

	Word address;

	switch (reg) {
	case 0b000:
		state->B = data;
		break;
	case 0b001:
		state->C = data;
		break;
	case 0b010:
		state->D = data;
		break;
	case 0b011:
		state->E = data;
		break;
	case 0b100:
		state->H = data;
		break;
	case 0b101:
		state->L = data;
		break;
	case 0b110:
		address = (state->H << 8) | state->L;
		state->mem[address] = data;
		break;
	case 0b111:
		state->A = data;
		break;
	}
}

void ORA(Byte opcode) { //1 Byte, 4 Cycles (7 for M), SZAcPC.
	Byte srcReg = (opcode << 5) >> 5;

	Byte src = 0x0;
	Word srcAddress = 0x0;
	switch (srcReg) {
	case 0b111: //A
		src = state->A;
		break;
	case 0b000: //B
		src = state->B;
		break;
	case 0b001: //C
		src = state->C;
		break;
	case 0b010: //D
		src = state->D;
		break;
	case 0b011: //E
		src = state->E;
		break;
	case 0b100: //H
		src = state->H;
		break;
	case 0b101: //L
		src = state->L;
		break;
	case 0b110: //M
		srcAddress = (state->H << 8) | state->L;
		break;
	}

	if (srcReg == 0b110)
		state->A |= (state->mem[srcAddress]);
	else
		state->A |= src;

	state->carry = 0;

	determineFlag(1, 1, 1, 1, 0, state->A);
}

void ORI() { //2 Bytes, 7 Cycles, SZAcPC.
	Byte value = state->mem[state->PC];
	state->PC++;

	state->A |= value;
	determineFlag(1, 1, 1, 1, 1, state->A);
}

void POP(Byte opcode) { //1 Byte, 10 Cycles (SZeAcPCy for PSW).
	Byte reg = (opcode << 2) >> 6;

	Byte lsb = 0b0;
	Byte msb = 0b0;

	Byte F = 0b0;

	switch (reg) {
	case 0b00: //B
		state->C = state->mem[state->SP], state->SP++;
		state->B = state->mem[state->SP], state->SP++;
		break;
	case 0b01: //D
		state->E = state->mem[state->SP], state->SP++;
		state->D = state->mem[state->SP], state->SP++;
		break;
	case 0b10: //H
		state->L = state->mem[state->SP], state->SP++;
		state->H = state->mem[state->SP], state->SP++;
		break;
	case 0b11: {
		F = state->mem[state->SP], state->SP++;

		state->sign = (F >> 7);
		state->zero = (F << 1) >> 7;
		state->auxillaryCarry = (F >> 3) >> 7;
		state->parity = (F << 5) >> 7;
		state->carry = (F << 7) >> 7;

		state->A = state->mem[state->SP], state->SP++;
		break;
	}
	}
}

void PUSH(Byte opcode) { //1 Byte, 11 Cycles.
	Byte reg = (opcode << 2) >> 6;

	Byte lsb = 0b0;
	Byte msb = 0b0;

	Byte F = 0b0;

	switch (reg) {
	case 0b00: //B
		state->C = state->mem[state->SP - 2];
		state->B = state->mem[state->SP - 1];
		break;
	case 0b01: //D
		state->E = state->mem[state->SP - 2];
		state->D = state->mem[state->SP - 1];
		break;
	case 0b10: //H
		state->L = state->mem[state->SP - 2];
		state->H = state->mem[state->SP - 1];
		break;
	case 0b11: {
		F = state->mem[state->SP - 2];

		state->sign = (F >> 7);
		state->zero = (F << 1) >> 7;
		state->auxillaryCarry = (F >> 3) >> 7;
		state->parity = (F << 5) >> 7;
		state->carry = (F << 7) >> 7;

		state->A = state->mem[state->SP - 1];
		break;
	}
	}

	state->SP -= 2;
}

void RAL() { //1 Byte, 4 Cycles, C.
	Byte tmp = state->A;

	state->A = (state->A << 1) | state->carry;
	state->carry = (tmp >> 7);
}

void RAR() { //1 Byte, 4 Cycles, C.
	Byte tmp = state->A;

	state->A = (state->A >> 1) | (state->carry << 7);
	state->carry = (tmp << 7) >> 7;
}

void RET() { // 1Byte, 10 Cycles.
	state->PC = (state->mem[state->SP + 1] << 8) | state->mem[state->SP];
	state->SP += 2;
}

void RLC() { //1 Byte, 4 Cycles, C.
	Byte tmp = state->A;
	state->carry = (tmp >> 7);

	state->A = tmp << 1 | tmp >> 7;
}

void RRC() { //1 Byte, 4 Cycles, C.
	Byte tmp = state->A;
	state->carry = (tmp << 7) >> 7;

	state->A = tmp >> 1 | (tmp << 7) >> 7;
}

void RST(Byte opcode) { //1 Byte, 11 Cycles.
	Byte exp = (opcode << 2) >> 5;

	Word tempPC = state->PC;
	state->mem[state->SP - 1] = (tempPC >> 8);
	state->mem[state->SP - 2] = (tempPC << 8) >> 8;
	state->SP -= 2;

	state->PC = exp * 0x8;
}

void SBB(Byte opcode) { //1 Byte, 4 Cycles (7 for M), SZAcPC.
	Byte srcReg = (opcode << 5) >> 5;

	Byte src = 0x0;
	Word srcAddress = 0x0;
	switch (srcReg) {
	case 0b111: //A
		src = state->A;
		break;
	case 0b000: //B
		src = state->B;
		break;
	case 0b001: //C
		src = state->C;
		break;
	case 0b010: //D
		src = state->D;
		break;
	case 0b011: //E
		src = state->E;
		break;
	case 0b100: //H
		src = state->H;
		break;
	case 0b101: //L
		src = state->L;
		break;
	case 0b110: //M
		srcAddress = (state->H << 8) | state->L;
		break;
	}

	src += state->carry;

	if (srcReg == 0b110)
		state->A -= (state->mem[srcAddress] + state->carry);
	else
		state->A -= src;

	determineFlag(1, 1, 1, 1, 1, state->A);
}

void SBI() { //2 Bytes, 7 Cycles, SZAcPC.
	Byte value = state->mem[state->PC];
	state->PC++;

	state->A -= value - state->carry;
	determineFlag(1, 1, 1, 1, 1, state->A);
}

void SHLD() { //3 Bytes, 16 Cycles.
	Word address = state->mem[state->PC];
	address = state->mem[state->PC++] << 7;
	state->PC++;
	state->mem[address] = state->L;
	state->mem[address + 1] = state->H;
}

void STA() { //3 Byte, 13 Cycles.
	Word address = state->mem[state->PC] | (state->mem[state->PC++] << 8);
	state->PC++;

	state->mem[address] = state->A;
}

void STAX(Byte opcode) { //1 Byte, 7 Cycles.
	Byte regPair = (opcode << 2) >> 6;
	Word address;

	switch (regPair) {
	case 0b00: //BC Pair
		address = state->B << 8;
		address |= state->C;
		state->mem[address] = state->A;
		break;
	case 0b01: //DE Pair
		address = state->D << 8;
		address |= state->E;
		state->mem[address] = state->A;
		break;
	}
}

void SUB(Byte opcode) { //1 Byte, 4 Cycles (7 for M), SZAcPC.
	Byte srcReg = (opcode << 5) >> 5;

	Byte src = 0x0;
	Word srcAddress = 0x0;
	switch (srcReg) {
	case 0b111: //A
		src = state->A;
		break;
	case 0b000: //B
		src = state->B;
		break;
	case 0b001: //C
		src = state->C;
		break;
	case 0b010: //D
		src = state->D;
		break;
	case 0b011: //E
		src = state->E;
		break;
	case 0b100: //H
		src = state->H;
		break;
	case 0b101: //L
		src = state->L;
		break;
	case 0b110: //M
		srcAddress = (state->H << 8) | state->L;
		break;
	}

	if (srcReg == 0b110)
		state->A -= state->mem[srcAddress];
	else
		state->A -= src;

	determineFlag(1, 1, 1, 1, 1, state->A);
}

void SUI() { //2 Bytes, 7 Cycles, SZAcPC.
	Byte value = state->mem[state->PC];
	state->PC++;

	state->A -= value;
	determineFlag(1, 1, 1, 1, 1, state->A);
}

void XCHG() { //1 Byte, 5 Cycles.
	Byte temp[] = {
		state->D, state->E, state->H, state->L
	};

	state->D = temp[2];
	state->E = temp[3];

	state->H = temp[0];
	state->L = temp[1];
}

void XRA(Byte opcode) { //1 Byte, 4 Cycles (7 for M), SZeAcPCy.
	Byte srcReg = (opcode << 5) >> 5;

	Byte src = 0x0;
	Word srcAddress = 0x0;
	switch (srcReg) {
	case 0b111: //A
		src = state->A;
		break;
	case 0b000: //B
		src = state->B;
		break;
	case 0b001: //C
		src = state->C;
		break;
	case 0b010: //D
		src = state->D;
		break;
	case 0b011: //E
		src = state->E;
		break;
	case 0b100: //H
		src = state->H;
		break;
	case 0b101: //L
		src = state->L;
		break;
	case 0b110: //M
		srcAddress = (state->H << 8) | state->L;
		break;
	}

	if (srcReg == 0b110)
		state->A ^= (state->mem[srcAddress]);
	else
		state->A ^= src;

	state->carry = 0;

	determineFlag(1, 1, 1, 1, 0, state->A);
}

void XRI() { //2 Bytes, 7 Cycles, SZAcPC.
	Byte value = state->mem[state->PC];
	state->PC++;

	state->A ^= value;
	determineFlag(1, 1, 1, 1, 1, state->A);
}
//END

int handleIns() {
	Byte opcode = state->mem[state->PC];
	int ins = opArraySearch(state->mem[state->PC]);
	state->PC++;

	switch (ins) {
	case ('ACI'): {
		ACI();
		break;
	}
	case ('ADC'): {
		ADC(opcode);
		break;
	}
	case ('ADD'): {
		ADD(opcode);
		break;
	}
	case ('ADI'): {
		ADI();
		break;
	}
	case ('ANA'): {
		ANA();
		break;
	}
	case ('ANI'): {
		ANI();
		break;
	}
	case ('CALL'): {
		CALL();
		break;
	}
	case ('CC'): {
		if (state->carry == 1) {
			cycleSwitches.Cccc = 17;
			CALL();
		}
		else { cycleSwitches.Cccc = 11; }
		break;
	}
	case ('CM'): {
		if (state->sign == 1) {
			cycleSwitches.Cccc = 17;
			CALL();
		}
		else { cycleSwitches.Cccc = 11; }
		break;
	}
	case ('CMA'): {
		CMA();
		break;
	}
	case ('CMC'): {
		CMC();
		break;
	}
	case ('CMP'): {
		CMP(opcode);
		break;
	}
	case ('CNC'): {
		if (state->carry == 0) {
			cycleSwitches.Cccc = 17;
			CALL();
		}
		else { cycleSwitches.Cccc = 11; }
		break;
	}
	case ('CNZ'): {
		if (state->zero == 0) {
			cycleSwitches.Cccc = 17;
			CALL();
		}
		else { cycleSwitches.Cccc = 11; }
		break;
	}
	case ('CP'): {
		if (state->sign == 0) {
			cycleSwitches.Cccc = 17;
			CALL();
		}
		else { cycleSwitches.Cccc = 11; }
		break;
	}
	case ('CPE'): {
		if (state->parity == 1) {
			cycleSwitches.Cccc = 17;
			CALL();
		}
		else { cycleSwitches.Cccc = 11; }
		break;
	}
	case ('CPI'): {
		CPI();
		break;
	}
	case ('CPO'): {
		if (state->parity == 0) {
			cycleSwitches.Cccc = 17;
			CALL();
		}
		else { cycleSwitches.Cccc = 11; }
		break;
	}
	case ('CZ'): {
		if (state->zero == 1) {
			cycleSwitches.Cccc = 17;
			CALL();
		}
		else { cycleSwitches.Cccc = 11; }
		break;
	}
	case ('DAA'): {
		DAA();
		break;
	}
	case ('DAD'): {
		DAD(opcode);
		break;
	}
	case ('DCR'): {
		DCR(opcode);
		break;
	}
	case ('DCX'): {
		DCX(opcode);
		break;
	}
	case ('DI'): {
		state->interruptEnable = false;
		break;
	}
	case ('EI'): {
		state->interruptEnable = false;
		break;
	}
	case ('HLT'): {
		//Just an exit for now...
		exit(0);
		break;
	}
	case ('IN'): {
		state->A = state->port[state->mem[state->PC]]; state->PC++;
		break;
	}
	case ('INR'): {
		INR(opcode);
		break;
	}
	case ('INX'): {
		INX(opcode);
		break;
	}
	case ('JC'): {
		if (state->carry == 1) {
			JMP();
		}
		break;
	}
	case ('JM'): {
		if (state->sign == 1) {
			JMP();
		}
		break;
	}
	case ('JMP'): {
		JMP();
		break;
	}
	case ('JNC'): {
		if (state->carry == 0) {
			JMP();
		}
		break;
	}
	case ('JNZ'): {
		if (state->zero == 0) {
			JMP();
		}
		break;
	}
	case ('JP'): {
		if (state->sign == 0) {
			JMP();
		}
		break;
	}
	case ('JPE'): {
		if (state->parity == 1) {
			JMP();
		}
		break;
	}
	case ('JPO'): {
		if (state->parity == 0) {
			JMP();
		}
		break;
	}
	case ('JZ'): {
		if (state->zero == 1) {
			JMP();
		}
		break;
	}
	case ('LDA'): {
		Word address = state->mem[state->PC] | (state->mem[state->PC + 1] << 8); state->PC += 2;
		state->A = state->mem[address];
		break;
	}
	case ('LDAX'): {
		LDAX(opcode);
		break;
	}
	case ('LHLD'): {
		LHLD();
		break;
	}
	case ('LXI'): {
		LXI(opcode);
		break;
	}
	case ('MOV'): {
		MOV(opcode);
		break;
	}
	case ('MVI'): {
		MVI(opcode);
		break;
	}
	case ('NOP'): {
		break;
	}
	case ('ORA'): {
		ORA(opcode);
		break;
	}
	case ('ORI'): {
		ORI();
		break;
	}
	case ('OUT'): {
		state->port[state->mem[state->PC]] = state->A;
		break;
	}
	case ('PCHL'): {
		state->PC = (state->H << 8) | state->L;
		break;
	}
	case ('POP'): {
		POP(opcode);
		break;
	}
	case ('PUSH'): {
		PUSH(opcode);
		break;
	}
	case ('RAL'): {
		RAL();
		break;
	}
	case ('RAR'): {
		RAR();
		break;
	}
	case ('RC'): {
		if (state->carry == 1) {
			RET();
			cycleSwitches.Rccc = 11;
		}
		else { cycleSwitches.Rccc = 5; }
		break;
	}
	case ('RET'): {
		RET();
		break;
	}
	case ('RLC'): {
		RLC();
		break;
	}
	case ('RM'): {
		if (state->sign == 1) {
			RET();
			cycleSwitches.Rccc = 11;
		}
		else { cycleSwitches.Rccc = 5; }
		break;
	}
	case ('RNC'): {
		if (state->carry == 0) {
			RET();
			cycleSwitches.Rccc = 11;
		}
		else { cycleSwitches.Rccc = 5; }
		break;
	}
	case ('RNZ'): {
		if (state->zero == 0) {
			RET();
			cycleSwitches.Rccc = 11;
		}
		else { cycleSwitches.Rccc = 5; }
		break;
	}
	case ('RP'): {
		if (state->sign == 0) {
			RET();
			cycleSwitches.Rccc = 11;
		}
		else { cycleSwitches.Rccc = 5; }
		break;
	}
	case ('RPE'): {
		if (state->parity == 1) {
			RET();
			cycleSwitches.Rccc = 11;
		}
		else { cycleSwitches.Rccc = 5; }
		break;
	}
	case ('RPO'): {
		if (state->parity == 0) {
			RET();
			cycleSwitches.Rccc = 11;
		}
		else { cycleSwitches.Rccc = 5; }
		break;
	}
	case ('RRC'): {
		RRC();
		break;
	}
	case ('RST'): {
		RST(opcode);
		break;
	}
	case ('RZ'): {
		if (state->zero == 1) {
			RET();
			cycleSwitches.Rccc = 11;
		}
		else { cycleSwitches.Rccc = 5; }
		break;
	}
	case ('SBB'): {
		SBB(opcode);
		break;
	}
	case ('SBI'): {
		SBI();
		break;
	}
	case ('SHLD'): {
		SHLD();
		break;
	}
	case ('SPHL'): {
		state->SP = (state->H << 8) | state->L;
		break;
	}
	case ('STA'): {
		STA();
		break;
	}
	case ('STAX'): {
		STAX(opcode);
		break;
	}
	case ('STC'): {
		state->carry = 1;
		break;
	}
	case ('SUB'): {
		SUB(opcode);
		break;
	}
	case ('SUI'): {
		SUI();
		break;
	}
	case ('XCHG'): {
		XCHG();
		break;
	}
	case ('XRA'): {
		XRA(opcode);
		break;
	}
	case ('XRI'): {
		XRI();
		break;
	}
	case ('XTHL'): {
		state->L = state->mem[state->SP];
		state->H = state->mem[state->SP + 1];
		break;
	}
	}

	return CPUCycles[opcode];
}

void procClear() {
	delete state;
}