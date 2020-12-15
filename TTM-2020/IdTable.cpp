#include "pch.h"
#include "IdTable.h"
#include "Error.h"

TTM::IdTable::Entry::Entry(std::string name, std::string scope, int lexTableIndex, it::id_type idType, int value)
	: name(name), scope(scope), lexTableIndex(lexTableIndex), dataType(it::data_type::i32), idType(idType)
{
	this->value.intValue = value;
}

TTM::IdTable::Entry::Entry(std::string name, std::string scope, int lexTableIndex, it::id_type idType, const char* value)
	: name(name), scope(scope), lexTableIndex(lexTableIndex), dataType(it::data_type::str), idType(idType)

{
	setValue(value);
}

TTM::IdTable::Entry::Entry(std::string name, std::string scope, int lexTableIndex, it::data_type dataType, it::id_type idType, const char* value)
	: name(name), scope(scope), lexTableIndex(lexTableIndex), dataType(dataType), idType(idType)

{
	if (dataType == it::data_type::i32)
		this->value.intValue = atoi(value);
	else
		setValue(value);
}

void TTM::IdTable::Entry::setValue(int new_value)
{
	if (dataType == it::data_type::i32)
		value.intValue = new_value;
}

void TTM::IdTable::Entry::setValue(const char* new_value)
{
	if (dataType == it::data_type::str)
	{
		value.strValue.length = static_cast<char>(strlen(new_value)) - 2;
		strcpy_s(value.strValue.string, new_value);
	}
}

int TTM::IdTable::addEntry(const Entry& entry)
{
	if (entry.idType == it::id_type::unknown)
		throw ERROR_THROW(120);
	if (entry.dataType == it::data_type::undefined)
		throw ERROR_THROW(121);

	m_table.push_back(entry);
	return m_table.size() - 1;
}

int TTM::IdTable::getIdIndexByName(std::string scope, std::string name)
{
	for (size_t i = 0; i < m_table.size(); ++i)
	{
		if (m_table[i].scope == scope && m_table[i].name == name)
		{
			return i;
		}
	}

	return TI_NULLIDX;
}

int TTM::IdTable::getLiteralIndexByValue(int value) {
	for (size_t i = 0; i < m_table.size(); ++i)
	{
		if (m_table[i].idType == it::id_type::literal && m_table[i].value.intValue == value)
		{
			return i;
		}
	}

	return TI_NULLIDX;
}

int TTM::IdTable::getLiteralIndexByValue(const char* value) {
	for (size_t i = 0; i < m_table.size(); ++i)
	{
		if (m_table[i].idType == it::id_type::literal && !strcmp(m_table[i].value.strValue.string, value))
		{
			return i;
		}
	}

	return TI_NULLIDX;
}

const std::string TTM::IdTable::dumpTable(size_t startIndex, size_t endIndex) const
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

	output << std::setw(5) << "index " << std::setw(15) << "name " << std::setw(15) << "scope "
		<< std::setw(8) << "type " << std::setw(15) << "value " << std::setw(20) << "id type " << std::setw(20) << "lexTable index " << '\n';
	for (size_t i = startIndex; i < endIndex; ++i)
	{
		output << std::setw(5) << i << std::setw(15) << m_table[i].name << std::setw(15) << m_table[i].scope
			<< std::setw(8);
		if (m_table[i].dataType == it::data_type::i32)
			output << "i32 " << std::setw(15) << m_table[i].value.intValue;
		else if (m_table[i].dataType == it::data_type::str)
			output << "str " << std::setw(15) << m_table[i].value.strValue.string;

		output << std::setw(20);
		if (m_table[i].idType == it::id_type::function)
			output << "function";
		else if (m_table[i].idType == it::id_type::literal)
			output << "literal";
		else if (m_table[i].idType == it::id_type::parameter)
			output << "parameter";
		else if (m_table[i].idType == it::id_type::variable)
			output << "variable";

		output << std::setw(20) << m_table[i].lexTableIndex;
		output << '\n';
	}

	return output.str();
}