#include "pch.h"
#include "LexTable.h"
#include "Error.h"

TTM::LexTable::LexTable(size_t capacity)
	: m_size(0), m_table()
{
	m_table.reserve(capacity);
}

void TTM::LexTable::AddEntry(const Entry& entry)
{
	m_table.push_back(entry);
	++m_size;
}

TTM::LexTable::Entry& TTM::LexTable::operator[](size_t index)
{
	if (index < 0 || index > m_size - 1)
		throw ERROR_THROW(122);

	return m_table[index];
}