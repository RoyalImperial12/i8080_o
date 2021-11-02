#include "processor.h"
#include <iostream>
#include <map>

using namespace i8080;

// All this at the top here is to avoid doing some comically long switch statement when handling instructions.
// Believe me, another ~500-line switch statement which seperates each opcode into an indivdual case is not something I want to do again...
// Especially when the functions that actually emulates the instruction will identify the appropriate registers anyway.

char opMnemonic[] = { 'ACI', 'ADC', 'ADD', 'ADI', 'ANA', 'ANI', 
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

Byte ACI[] = { 0xCE };
Byte ADC[] = { 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F };
Byte ADD[] = { 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87 };
Byte ADI[] = { 0xC6 };
Byte ANA[] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7 };
Byte ANI[] = { 0xE6 };
Byte CALL[] = { 0xCD, 0xDD, 0xED, 0xFD };
Byte CC[] = { 0xDC };
Byte CM[] = { 0xFC };
Byte CMA[] = { 0x2F };
Byte CMC[] = { 0x3F };
Byte CMP[] = { 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF };
Byte CNC[] = { 0xD4 };
Byte CNZ[] = { 0xC4 };
Byte CP[] = { 0xF4 };
Byte CPE[] = { 0xEC };
Byte CPI[] = { 0xFE };
Byte CPO[] = { 0xE4 };
Byte CZ[] = { 0xCC };
Byte DAA[] = { 0x27 };
Byte DAD[] = { 0x09, 0x19, 0x29, 0x39 };
Byte DCR[] = { 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x35, 0x3D };
Byte DCX[] = { 0x0B, 0x1B, 0x2B, 0x3B };
Byte DI[] = { 0xF3 };
Byte EI[] = { 0xFB };
Byte HLT[] = { 0x76 };
Byte IN[] = { 0xDB };
Byte INR[] = { 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C };
Byte INX[] = { 0x03, 0x13, 0x23, 0x33 };
Byte JC[] = { 0xDA };
Byte JM[] = { 0xFA };
Byte JMP[] = { 0xC3, 0xCB };
Byte JNC[] = { 0xD2 };
Byte JNZ[] = { 0xC2 };
Byte JP[] = { 0xF2 };
Byte JPE[] = { 0xEA };
Byte JPO[] = { 0xE2 };
Byte JZ[] = { 0xCA };
Byte LDA[] = { 0x3A };
Byte LDAX[] = { 0x0A, 0x1A };
Byte LHLD[] = { 0x2A };
Byte LXI[] = { 0x01, 0x11, 0x21, 0x31 };
Byte MOV[] = { 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
			   0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 
			   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 
			   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F };
Byte MVI[] = { 0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x36, 0x3E };
Byte NOP[] = { 0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38 };
Byte ORA[] = { 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7 };
Byte ORI[] = { 0xF6 };
Byte OUT[] = { 0xD3 };
Byte PCHL[] = { 0xE9 };
Byte POP[] = { 0xC1, 0xD1, 0xE1, 0xF1 };
Byte PUSH[] = { 0xC5, 0xD5, 0xE5, 0xF5 };
Byte RAL[] = { 0x17 };
Byte RAR[] = { 0x1F };
Byte RC[] = { 0xD8 };
Byte RET[] = { 0xC9, 0xD9 };
Byte RLC[] = { 0x07 };
Byte RM[] = { 0xF8 };
Byte RNC[] = { 0xD0 };
Byte RNZ[] = { 0xC0 };
Byte RP[] = { 0xF0 };
Byte RPE[] = { 0xE8 };
Byte RPO[] = { 0xE0 };
Byte RRC[] = { 0x0F };
Byte RST[] = { 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF };
Byte RZ[] = { 0xC8 };
Byte SBB[] = { 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F };
Byte SBI[] = { 0xDE };
Byte SHLD[] = { 0x22 };
Byte SPHL[] = { 0xF9 };
Byte STA[] = { 0x32 };
Byte STAX[] = { 0x02, 0x12 };
Byte STC[] = { 0x37 };
Byte SUB[] = { 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97 };
Byte SUI[] = { 0xD6 };
Byte XCHG[] = { 0xEB };
Byte XRA[] = { 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF };
Byte XRI[] = { 0xEE };
Byte XTHL[] = { 0xE3 };

std::map<char, Byte*> op{ {'ACI', ACI}, {'ADC', ADC}, {'ADD', ADD}, {'ADI', ADI}, {'ANA', ANA}, {'ANI', ANI},
						  {'CALL', CALL}, {'CC', CC}, {'CM', CM}, {'CMA', CMA}, {'CMC', CMC}, {'CMP', CMP}, {'CNC', CNC}, {'CNZ', CNZ}, {'CP', CP}, {'CPE', CPE}, {'CPI', CPI}, {'CPO', CPO}, {'CZ', CZ},
						  {'DAA', DAA}, {'DAD', DAD}, {'DCR', DCR}, {'DCX', DCX}, {'DI', DI},
						  {'EI', EI},
						  {'HLT', HLT},
						  {'IN', IN}, {'INR', INR}, {'INX', INX},
						  {'JC', JC}, {'JM', JM}, {'JMP', JMP}, {'JNC', JNC}, {'JNZ', JNZ}, {'JP', JP}, {'JPE', JPE}, {'JPO', JPO}, {'JZ', JZ},
						  {'LDA', LDA}, {'LDAX', LDAX}, {'LHLD', LHLD}, {'LXI', LXI},
						  {'MOV', MOV}, {'MVI', MVI},
						  {'NOP', NOP},
						  {'ORA', ORA}, {'ORI', ORI},
						  {'OUT', OUT},
						  {'PCHL', PCHL}, {'POP', POP}, {'PUSH', PUSH},
						  {'RAL', RAL}, {'RAR', RAR}, {'RC', RC}, {'RET', RET}, {'RLC', RLC}, {'RM', RM}, {'RNC', RNC}, {'RNZ', RNZ}, {'RP', RP}, {'RPE', RPE}, {'RPO', RPO}, {'RRC', RRC}, {'RST', RST}, {'RZ', RZ},
						  {'SBB', SBB}, {'SBI', SBI}, {'SHLD', SHLD}, {'SPHL', SPHL}, {'STA', STA}, {'STAX', STAX}, {'STC', STC}, {'SUB', SUB}, {'SUI', SUI},
						  {'XCHG', XCHG}, {'XRA', XRA}, {'XRI', XRI}, {'XTHL', XTHL} };

void handleIns(i8080State* state) {

}