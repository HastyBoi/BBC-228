#pragma once
#include "LexTable.h"
#include "IdTable.h"

#define FORBIDDEN_SYMBOL '\0'

namespace TTM
{
	class PolishNotation
	{
	public:
		PolishNotation(LexTable& lextable, IdTable& idtable);

	private:
		LexTable& lextable;
		IdTable& idtable;

		int getOperationsPriority(char operation);
		bool shokk(int startIndex);
		std::vector<LexTable::Entry> convert(const std::vector<LexTable::Entry>& entries);
	};
}
