#include "pch.h"
#include "SemanticAnalyzer.h"
#include "Error.h"

TTM::SemanticAnalyzer::SemanticAnalyzer(LexTable& lextable, IdTable& idtable)
	: lextable(lextable), idtable(idtable)
{
	getAllFunctionParameters();
}

void TTM::SemanticAnalyzer::Start(Logger& log)
{
	checkAssignmentTypeMismatch();
	checkReturnTypeMismatch();
	checkArgumentsAmountMismatch();

	log << "—емантический анализ выполнен без ошибок\n";
}

void TTM::SemanticAnalyzer::checkAssignmentTypeMismatch()
{
	for (int i = 0; i < lextable.size(); ++i)
	{
		if (lextable[i].lexeme == LEX_ASSIGN)
		{
			auto& previousEntry = idtable[lextable[i - 1].idTableIndex];
			auto& nextEntry = idtable[lextable[i + 1].idTableIndex];

			if (previousEntry.dataType != nextEntry.dataType)
				throw ERROR_THROW_IN(706, lextable[i].lineNumber, -1);

			if (previousEntry.idType == it::id_type::variable
				&& nextEntry.idType == it::id_type::literal)
			{
				previousEntry.value = nextEntry.value;
			}
		}
	}
}

void TTM::SemanticAnalyzer::checkReturnTypeMismatch()
{
	auto returnDataType = it::data_type::undefined;

	for (int i = 0; i < lextable.size(); ++i)
	{
		if (lextable[i].lexeme == LEX_RET)
		{
			returnDataType = idtable[lextable[i + 1].idTableIndex].dataType;
			for (int j = i; j > 0; --j)
			{
				if (lextable[j].lexeme == LEX_FN)
				{
					if (returnDataType != idtable[lextable[j + 2].idTableIndex].dataType)
						throw ERROR_THROW_IN(700, lextable[i].lineNumber, -1);
				}
			}
		}
	}
}

void TTM::SemanticAnalyzer::checkArgumentsAmountMismatch()
{
}

void TTM::SemanticAnalyzer::getAllFunctionParameters()
{
}