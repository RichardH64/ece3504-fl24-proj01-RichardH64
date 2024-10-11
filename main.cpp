#include "Disassembler.hpp"

int main(int argc, char* argv[]) 
{
	Disassembler myDisassembler;

	std::cout << "Disassembling " << argv[1] << "..." << std::endl;

	myDisassembler.Disassemble(argv[1]);


	return 0;
}