#include "ErrorHandler.hpp"

#include <unordered_map>


class Disassembler
{
private:
	ErrorHandler* m_handler = nullptr;

	size_t m_size = 0;
	size_t m_capacity = 2;
	std::string* m_instructions = nullptr;

	std::unordered_map<size_t, std::string> m_labels;


	void reset();
	void ensureCapacity();
	void saveLabel(const std::string&);
	bool insert(const size_t&, const std::string&);

	void RTypeDisassembler(const std::string&, const std::string&);
	void ITypeDisassembler(const std::string&, const std::string&);
	void BaseDisassembler(const std::string&);
public:
	void Disassemble(const std::string&);

	Disassembler();
	~Disassembler();
};