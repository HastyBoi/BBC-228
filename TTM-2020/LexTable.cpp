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

//todo dumpTable
std::string TTM::LexTable::dumpTable(size_t startIndex, size_t endIndex) const
{
	std::stringstream output;

	startIndex = std::clamp(startIndex, 0U, m_table.size());
	endIndex = std::clamp(endIndex, startIndex, m_table.size());

	output << "under construction\n";

	for (size_t i = startIndex; i < endIndex; ++i)
	{
		output << std::setw(3) << i << std::setw(8) << '\'' << m_table[i].lexeme << '\'';
		if (m_table[i].idTableIndex != TI_NULLIDX)
		{
			output << std::setw(10) << m_table[i].idTableIndex;
		}
		output << '\n';
	}

	return output.str();
}