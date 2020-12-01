#include "pch.h"
#include "In.h"
#include "Error.h"
#include "IT.h"

TTM::InputFileReader::InputFileReader() :
	m_fileSize(0), m_linesCount(1), m_ignoredCharsCount(0), m_fileText(""), m_codeTable{ IN_CODE_TABLE }
{	}

void TTM::InputFileReader::read(const char* inFilePath)
{
	std::ifstream inputFile(inFilePath, std::ios::ate);

	if (!inputFile.is_open())
		throw ERROR_THROW(110);

	m_fileText.reserve((size_t)inputFile.tellg());
	inputFile.seekg(std::ios::beg);

	for (int readChar = 0, column = 0; (readChar = inputFile.get()) != EOF; ++m_fileSize)
	{
		++column;
		if (readChar == IN_CODE_ENDL) {
			++m_linesCount;
			column = 0;
		}

		switch (m_codeTable[readChar])
		{
		case IN::T:
			m_fileText.push_back(readChar);
			break;

		case IN::Q:
			do
			{
				m_fileText.push_back(readChar);
				readChar = inputFile.get();
				++m_fileSize;
				++column;
			} while (m_codeTable[readChar] != IN::Q && column <= TI_STR_MAXSIZE);
			m_fileText.push_back(readChar);
			break;

		case IN::O:
			if (!m_fileText.empty() && m_fileText.back() != IN_CODE_DELIM)
				m_fileText.push_back(IN_CODE_DELIM);
			m_fileText.push_back(readChar);
			m_fileText.push_back(IN_CODE_DELIM);
			break;

		case IN::I:
			++m_ignoredCharsCount;
			break;

		case IN::F:
			throw ERROR_THROW_IN(111, m_linesCount, column);
			break;

		default:
			if (!m_fileText.empty() && m_fileText.back() != IN_CODE_DELIM)
				m_fileText.push_back(m_codeTable[readChar]);
			break;
		}
	}

	inputFile.close();
}

std::vector<std::string_view> TTM::InputFileReader::splitStringByDelimiter(std::string_view s, char delimiter)
{
	std::vector<std::string_view> output;
	output.reserve(std::count_if(s.begin(), s.end(), [&](char c) { return c == delimiter; }) + 1);

	size_t start = 0;
	size_t end = s.find(delimiter);

	while (end != std::string_view::npos)
	{
		output.push_back(s.substr(start, end - start));
		start = end + 1;
		end = s.find(delimiter, start);
	}

	std::string_view last_element = s.substr(start, end);
	if (!last_element.empty() && last_element.find(delimiter) == std::string_view::npos)
		output.push_back(s.substr(start, end));

	return output;
}