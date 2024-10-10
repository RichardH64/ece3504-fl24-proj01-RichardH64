#include "Disassembler.hpp"
#include "ErrorHandler.hpp"
#include "Utilities.hpp"

#include <fstream>


void Disassembler::reset()
{
	m_handler->reset();

	m_size = 0;
	m_capacity = 2;

	delete[] m_instructions;
	m_instructions = new std::string[m_capacity];
}

void Disassembler::ensureCapacity()
{
	if (m_instructions == nullptr)
	{
		m_capacity = 2;

		m_instructions = new std::string[m_capacity];
	}
	// Dynamically reallocates space for the array
	if (m_size >= m_capacity)
	{
		m_capacity *= 2;

		std::string* m_newdata = new std::string[m_capacity];
		if (!m_newdata) {
			std::cerr << "Memory allocation failed!" << std::endl;
			return;
		}

		for (size_t i = 0; i < m_size; i++)
		{
			m_newdata[i] = std::move(m_instructions[i]);
		}

		delete[] m_instructions;
		m_instructions = m_newdata;
	}
}

void Disassembler::BaseDisassembler(const std::string& hex)
{
	// Step 1: Checks if there are 8 hexadecimals (if not then the hex is invalid)
	if (hex.size() != 8) {
		m_handler->addError(hex);
		return; // Stops the disassembler
	}

	// Step 2: Converts each hex to binary (any invalid hex char will cause an error)
	std::string binaryString = "";
	for (char hexChar : hex) {
		std::string nibble = hexCharToBinary(hexChar);
		if (nibble == "xxxx") {
			m_handler->addError(hex);
			return; // Stops the disassembler
		}
		binaryString += nibble;
	}

	if (binaryString.substr(START_OPCODE, LENGTH_OPCODE) == "000000") {
		RTypeDisassembler(hex, binaryString);
	} 
	else {
		ITypeDisassembler(hex, binaryString);
	}
}

void Disassembler::RTypeDisassembler(const std::string& hex, const std::string& binary)
{
	std::string instruction = "";

	// Step 3: Dissect binary values (opcode not needed)
	std::string rs = binary.substr(START_RS, LENGTH_RS), rt = binary.substr(START_RT, LENGTH_RT), rd = binary.substr(START_RD, LENGTH_RD);
	std::string shamt = binary.substr(START_SHAMT, LENGTH_SHAMT), funct = binary.substr(START_FUNCT, LENGTH_FUNCT);


	// Step 4: Add the mnemonic, because it's R-Type, based off FUNCT 6 bits
	// Checks if the 6-bit code is in the map for mnemonics
	if (R_TYPE_DICTIONARY.find(funct) == R_TYPE_DICTIONARY.end()) {
		m_handler->addError(hex);
		return; // Stops the disassembler
	}
	instruction += R_TYPE_DICTIONARY.at(funct) + " ";

	// Step 5: Add registers (Check if SHAMT is involved)
	// RD in every r-type
	instruction += REGISTER_DICTIONARY.at(rd) + ", ";
	if (funct == "000000" || funct == "000010")
	{
		instruction += REGISTER_DICTIONARY.at(rt) + ", ";
		instruction += std::to_string(binaryToUInt(shamt));
	}
	else
	{
		instruction += REGISTER_DICTIONARY.at(rs) + ", ";
		instruction += REGISTER_DICTIONARY.at(rt) + " ";
	}

	// Step 6: Add instruction to list
	ensureCapacity();
	m_instructions[m_size] = instruction;
	m_size++;
}

void Disassembler::ITypeDisassembler(const std::string&, const std::string&)
{

}

void Disassembler::Disassemble(const std::string& filename)
{
	reset();

	// Step 1: Opens input file
	std::ifstream inputFile(filename);
	std::string hex; // The line should be 8 hexadecimals
	if (!inputFile.is_open()) {
		std::cerr << "Error: Could not open file \"" << filename << "\"" << std::endl;
		return;
	}

	// Step 2: Iterates through file
	while (std::getline(inputFile, hex)) {
		BaseDisassembler(hex);
		m_handler->nextLine();
	}
	inputFile.close(); // Making sure to close the file

	// Step 3: Checks if file had errors, if the file did then the errors will be printed to the command line and dissasembler terminated
	if (m_handler->getErrors() > 0) {
		m_handler->printErrors();
		return;
	}
	
	// Step 4: Creates the output file for 
}

Disassembler::Disassembler()
{
	m_handler = new ErrorHandler();
	m_instructions = new std::string[m_capacity];
}

Disassembler::~Disassembler()
{
	delete m_handler;
	delete m_instructions;
}