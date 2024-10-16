#include "Disassembler.hpp"

int main(int argc, char* argv[]) 
{
	Disassembler myDisassembler;

	std::cout << "Disassembling " << argv[1] << "..." << std::endl;

	for (int i = 1; i < argc; i++) {
		myDisassembler.Disassemble(argv[i]);
	}


	return 0;
}