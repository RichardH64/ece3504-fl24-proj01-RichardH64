#include "ErrorHandler.hpp"


void ErrorHandler::ensureCapacity()
{
	if (m_data == nullptr) 
	{
		m_capacity = 2;
		
		m_data = new std::string[m_capacity];
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
			m_newdata[i] = std::move(m_data[i]);
		}

		delete[] m_data;
		m_data = m_newdata;
	}
}

ErrorHandler::ErrorHandler() : m_capacity(DEFAULT_ERROR_SIZE), m_size(0), m_line(1)
{
	m_data = new std::string[m_capacity];
}

ErrorHandler::~ErrorHandler()
{
	delete[] m_data;
}

std::size_t ErrorHandler::getErrors() const noexcept
{
	return m_size;
}

void ErrorHandler::nextLine()
{
	m_line++;
}

void ErrorHandler::addError(const std::string& err)
{
	ensureCapacity();

	// Append the new error message
	m_data[m_size] = std::string("Cannot disassemble <" + err + "> at line <" + std::to_string(m_line) + ">");
	m_size++;
}

void ErrorHandler::printErrors()
{
	if (getErrors() == 0) {
		std::cout << "File was succesfully disassmbled with 0 errors" << std::endl;
		return;
	}

	std::cout << "Printing errors" << std::endl;
	for (size_t i = 0; i < m_size; i++) 
	{
		std::cout << "\t" << m_data[i] << std::endl;
	}
}
