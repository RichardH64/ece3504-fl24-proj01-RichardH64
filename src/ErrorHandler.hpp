#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#define DEFAULT_ERROR_SIZE 2

#include <string>
#include <iostream>


class ErrorHandler
{
private:
	size_t m_capacity;
	size_t m_size;    
	size_t m_line;    
	std::string* m_data;

	void ensureCapacity();
public:
	ErrorHandler();
	~ErrorHandler();

	void reset();

	size_t getErrors() const noexcept;
	void nextLine();
	void addError(const std::string&);
	void printErrors();
};

#endif // ERROR_HANDLER_HPP