#include "ErrorHandler.hpp"

class Disassembler 
{
private:
	ErrorHandler* m_handler = nullptr;

	size_t m_size = 0;
	size_t m_capacity = 2;
	std::string* m_instructions = nullptr;

	//void Helper_
	void ensureCapacity();

	void BaseDisassembler(const std::string&);
	void RTypeDisassembler(const std::string&, const std::string&);
	void ITypeDisassembler(const std::string&, const std::string&);

	//void FSM();
public:
	//void run();

	Disassembler();
	~Disassembler();
};