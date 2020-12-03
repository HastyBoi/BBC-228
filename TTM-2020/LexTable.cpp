#include "pch.h"
#include "LexTable.h"
#include "Error.h"

TTM::LexTable::LexTable(size_t capacity)
	: m_table()
{
	m_table.reserve(capacity);
}

void TTM::LexTable::addEntry(const LexTable::Entry& entry)
{
	m_table.push_back(entry);
}

TTM::LexTable::Entry::Entry(char lexeme, int lineNumber, int idTableIndex)
	: lexeme(lexeme), lineNumber(lineNumber), idTableIndex(idTableIndex)
{	}

const std::string TTM::LexTable::dumpTable(size_t startIndex, size_t endIndex) const
{
	std::stringstream output;

	startIndex = std::clamp(startIndex, 0U, m_table.size());
	if (endIndex == 0)
	{
		endIndex = m_table.size();
	}
	else
	{
		endIndex = std::clamp(endIndex, startIndex, m_table.size());
	}
	output << std::setw(4) << "index" << std::setw(20) << "lexeme" << std::setw(25) << "idTable index\n";

	for (size_t i = startIndex; i < endIndex; ++i)
	{
		output << std::setw(4) << i << std::setw(18) << '\'' << m_table[i].lexeme << '\'';
		if (m_table[i].idTableIndex != TI_NULLIDX)
		{
			output << std::setw(19) << m_table[i].idTableIndex;
		}
		output << '\n';
	}

	return output.str();
}

bool TTM::LexTable::hasLexeme(char lexeme) const
{
	for (const auto& e : m_table)
	{
		if (e.lexeme == lexeme)
			return true;
	}

	return false;
}