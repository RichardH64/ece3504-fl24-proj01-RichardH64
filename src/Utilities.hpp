#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#define START_OPCODE			0
#define LENGTH_OPCODE		6
#define START_RS				6
#define LENGTH_RS				5
#define START_RT				11
#define LENGTH_RT				5
#define START_RD				16
#define LENGTH_RD				5
#define START_SHAMT			21
#define LENGTH_SHAMT			5
#define START_FUNCT			26
#define LENGTH_FUNCT			6
#define START_IMMEDIATE		16
#define LENGTH_IMMEDIATE	16

#include <unordered_map>
#include <string>

const std::unordered_map<std::string, std::string> REGISTER_DICTIONARY
{
	{"00000", "$zero"}, {"00001", "$at"}, {"00010", "$v0"},
	{"00011", "$v1"}, {"00100", "$a0"}, {"00101", "$a1"},
	{"00110", "$a2"}, {"00111", "$a3"}, {"01000", "$t0"},
	{"01001", "$t1"}, {"01010", "$t2"}, {"01011", "$t3"},
	{"01100", "$t4"}, {"01101", "$t5"}, {"01110", "$t6"},
	{"01111", "$t7"}, {"10000", "$s0"}, {"10001", "$s1"},
	{"10010", "$s2"}, {"10011", "$s3"}, {"10100", "$s4"},
	{"10101", "$s5"}, {"10110", "$s6"}, {"10111", "$s7"},
	{"11000", "$t8"}, {"11001", "$t9"}, {"11010", "$k0"},
	{"11011", "$k1"}, {"11100", "$gp"}, {"11101", "$sp"},
	{"11110", "$fp"}, {"11111", "$ra"}
};

const std::unordered_map<std::string, std::string> R_TYPE_DICTIONARY
{
	{"100000", "add"}, {"100001", "addu"}, {"100100", "and"},
	{"100111", "nor"}, {"100101", "or "}, {"101010", "slt"},
	{"101011", "sltu"}, {"000000", "sll"}, {"000010", "srl"},
	{"100010", "sub"}, {"100011", "subu"}
};

const std::unordered_map<std::string, std::string> I_TYPE_DICTIONARY
{
	{"001000", "addi"}, {"001001", "addiu"}, {"001100", "andi"},
	{"000100", "beq"}, {"000101", "bne"}, {"100100", "lbu"},
	{"100101", "lhu"}, {"110000", "li "}, {"001111", "lui"},
	{"100011", "lw "}, {"001101", "ori"}, {"001010", "slti"},
	{"001011", "sltiu"}, {"101000", "sb "}, {"111000", "sc "},
	{"101001", "sh "}, {"101011", "sw "}
};


// Converts char to binary (in string format)
std::string hexCharToBinary(const char& hexChar) 
{   
	switch (hexChar) 
	{
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'A': case 'a': return "1010";
		case 'B': case 'b': return "1011";
		case 'C': case 'c': return "1100";
		case 'D': case 'd': return "1101";
		case 'E': case 'e': return "1110";
		case 'F': case 'f': return "1111";
		default: return "xxxx";
	}
}

// Converts binary to unsigned decimal
size_t binaryToUInt(const std::string& binary)
{
	int dec = 0;
	int place = 1;

	for (int i = binary.size() - 1; i >= 0; i--) {
		if (binary.at(i) == '1') {
			dec += place;
		}
		place *= 2;
	}

	return dec;
}

// Convers binary to signed decimal
ptrdiff_t binaryToInt(const std::string& binary)
{
	ptrdiff_t dec = 0;
	int place = 1;

	for (int i = binary.size() - 1; i > 0; i--) {
		if (binary.at(i) == '1') {
			dec += place;
		}
		place *= 2;
	}

	if (binary.at(0) == '1') {
		dec -= place;
	}

	return dec;
}

std::string intToHexString(int value)
{
	const char* hexDigits = "0123456789ABCDEF";
	std::string hexString = "0000";  // Prepare a fixed 4-character string

	// Iterate over the nibbles (4 bits) and convert each one
	for (int i = 3; i >= 0; --i) {
		hexString[3 - i] = hexDigits[(value >> (i * 4)) & 0xF];
	}

	return hexString;
}

#endif // !UTILITIES_HPP
