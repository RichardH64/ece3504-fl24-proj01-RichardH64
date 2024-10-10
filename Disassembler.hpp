#include "ErrorHandler.hpp"

class Disassembler 
{
private:
	ErrorHandler* m_handler = nullptr;

	size_t m_size = 0;
	size_t m_capacity = 2;
	std::string* m_instructions = nullptr;

	void reset();
	void ensureCapacity();

	void RTypeDisassembler(const std::string&, const std::string&);
	void ITypeDisassembler(const std::string&, const std::string&);
	void BaseDisassembler(const std::string&);
public:
	void Disassemble(const std::string&);

	Disassembler();
	~Disassembler();
};