#include "processor.h"
#include <iostream>
#include <map>

using namespace i8080;

// All this at the top here is to avoid doing some comedically long switch statement when handling instructions.
// Believe me, another ~500-line switch statement is not something I want to do again...

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
Byte IN[] = {  };
Byte INR[] = {  };
Byte INX[] = {  };
Byte JC[] = {  };
Byte JM[] = {  };
Byte JMP[] = {  };
Byte JNC[] = {  };
Byte JNZ[] = {  };
Byte JP[] = {  };
Byte JPE[] = {  };
Byte JPO[] = {  };
Byte JZ[] = {  };
Byte LDA[] = {  };
Byte LDAX[] = {  };
Byte LHLD[] = {  };
Byte LXI[] = {  };
Byte MOV[] = {  };
Byte MVI[] = {  };
Byte NOP[] = {  };
Byte ORA[] = {  };
Byte ORI[] = {  };
Byte OUT[] = {  };
Byte PCHL[] = {  };
Byte POP[] = {  };
Byte PUSH[] = {  };

std::map<char, Byte*> op{ {'ACI', ACI}, {'ADC', ADC}, {'ADD', ADD} }

