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
	m_instructions[m_size] = "main:\n";
	m_size++;
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

void Disassembler::saveLabel(const std::string& hex)
{
	std::string label = "Addr_" + hex.substr(4, 4) + ":\n";

	std::string addrInBinary = "";
	for (size_t i = 4; i < hex.size(); i++) {
		addrInBinary += hexCharToBinary(hex.at(i));
	}

	m_labels.insert({ binaryToUInt(addrInBinary) / 4, label });
}

bool Disassembler::insert(const size_t& pos, const std::string& instruction)
{
	ensureCapacity();
	if (pos > m_size) { return false; } // Position not available

	if (pos == m_size) {
		m_instructions[m_size] = instruction;
		m_size++;
		return true;
	}

	for (size_t i = m_size; i > pos; i--) {
		m_instructions[i] = m_instructions[i - 1];
	}
	m_instructions[pos] = instruction;
	m_size++;

	return true;
}

void Disassembler::RTypeDisassembler(const std::string& hex, const std::string& binary)
{
	std::string instruction = "\t";

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
	instruction += "\n";

	// Step 6: Add instruction to list
	ensureCapacity();
	m_instructions[m_size] = instruction;
	m_size++;
}

void Disassembler::ITypeDisassembler(const std::string& hex, const std::string& binary)
{
	std::string instruction = "\t";

	// Step 3: Dissect binary values
	std::string rs = binary.substr(START_RS, LENGTH_RS), rt = binary.substr(START_RT, LENGTH_RT);
	std::string opcode = binary.substr(START_OPCODE, LENGTH_OPCODE), immediate = binary.substr(START_IMMEDIATE, LENGTH_IMMEDIATE);

	// Step 4: Add the mnemonic, because it's I-Type, based off OPCODE 6 bits
	// Checks if the 6-bit code is in the map for mnemonics
	if (I_TYPE_DICTIONARY.find(opcode) == I_TYPE_DICTIONARY.end()) {
		m_handler->addError(hex);
		return; // Stops the disassembler
	}
	instruction += I_TYPE_DICTIONARY.at(opcode) + " ";

	// Step 5: Add registers
	switch (binaryToUInt(opcode))
	{
	case 8:		// addi
	case 12:		// andi
	case 13:		// ori
	case 10:		// slti
		instruction += REGISTER_DICTIONARY.at(rt) + ", ";
		instruction += REGISTER_DICTIONARY.at(rs) + ", ";
		instruction += std::to_string(binaryToInt(immediate));
		break;
	case 9:		// addiu
	case 11:		// sltiu
		instruction += REGISTER_DICTIONARY.at(rt) + ", ";
		instruction += REGISTER_DICTIONARY.at(rs) + ", ";
		instruction += std::to_string(binaryToUInt(immediate));
		break;
	case 4:		// beq
	case 5:		// bne
		instruction += REGISTER_DICTIONARY.at(rs) + ", ";
		instruction += REGISTER_DICTIONARY.at(rt) + ", ";
		instruction += "Addr_" + hex.substr(4, 4);
		saveLabel(hex);
		break;
	case 36:		// lbu
	case 37:		// lhu
		instruction += REGISTER_DICTIONARY.at(rt) + ", ";
		instruction += std::to_string(binaryToUInt(immediate)) + "(" + REGISTER_DICTIONARY.at(rs) + ")";
		break;
	case 48:		// li
	case 15:		// lui
	case 35:		// lw
	case 40:		// sb
	case 56:		// sc
	case 41:		// sh
	case 43:		// sw
		instruction += REGISTER_DICTIONARY.at(rt) + ", ";
		instruction += std::to_string(binaryToInt(immediate)) + "(" + REGISTER_DICTIONARY.at(rs) + ")";
		break;
	default:
		break;
	}
	instruction += "\n";

	// Step 6: Add instruction to list
	ensureCapacity();
	m_instructions[m_size] = instruction;
	m_size++;
}

void Disassembler::BaseDisassembler(const std::string& hex)
{
	// Step 1: Checks if there are 8 hexadecimals (if not then the hex is invalid)
	if (hex.size() != 8) {
		m_handler->addError(hex);
		std::cout << "Size Error" << std::endl;
		std::cout << "Size: " << std::to_string(hex.size()) << std::endl;
		return; // Stops the disassembler
	}

	// Step 2: Converts each hex to binary (any invalid hex char will cause an error)
	std::string binaryString = "";
	for (char hexChar : hex) {
		std::string nibble = hexCharToBinary(hexChar);
		if (nibble == "xxxx") {
			m_handler->addError(hex);
			std::cout << "Char unable to convert from hex to binary" << std::endl;
			std::cout << "Hex char: " << hexChar << std::endl;
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

void Disassembler::Disassemble(const std::string& filename)
{
	reset();

	// Step 1: Opens input file
	if (filename.substr(filename.size() - 4) != ".obj") {
		std::cerr << "Error: File does not end in .obj" << std::endl;
		return;
	}
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

	// Step 4: Adds labels to the assembly file
	for (auto label : m_labels) {
		insert(label.first, label.second);
	}

	// Step 5: Creates the output file
	std::string outfilename = filename;
	outfilename.pop_back();
	outfilename.pop_back();
	outfilename.replace(outfilename.length() - 1, 1, "s");

	std::ofstream outFile(outfilename);

	if (!outFile.is_open()) {
		std::cerr << "Error: Could not open file \"" << outfilename << "\"" << std::endl;
		return;
	}

	outFile << ".globl\tmain\n";
	outFile << ".data\n";
	outFile << ".text\n\n";

	for (size_t i = 0; i < m_size; i++) {
		outFile << m_instructions[i];
	}

	outFile << "\n\tori $v0, $0, 10\n";
	outFile << "\tsyscall";

	outFile.close();
	std::cout << "\"" << outfilename << "\" successfully created" << std::endl;
}

Disassembler::Disassembler()
{
	m_handler = new ErrorHandler();
	m_instructions = new std::string[m_capacity];
	m_instructions[m_size] = "main:\n";
	m_size++;
}

Disassembler::~Disassembler()
{
	delete m_handler;
	delete[] m_instructions;
}