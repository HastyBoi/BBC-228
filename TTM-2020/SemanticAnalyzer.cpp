#include "pch.h"
#include "SemanticAnalyzer.h"
#include "Error.h"

TTM::SemanticAnalyzer::SemanticAnalyzer(LexTable& lextable, IdTable& idtable)
	: lextable(lextable), idtable(idtable)
{	}

void TTM::SemanticAnalyzer::Start(Logger& log)
{
	checkTypeMismatch();
	checkAccessingOutOfScopeId();
	checkArgumentsMismatch();
}

void TTM::SemanticAnalyzer::checkTypeMismatch()
{
	for (int i = 0; i < lextable.size(); ++i)
	{
		if (lextable[i].lexeme == LEX_ASSIGN)
		{
			if (idtable[lextable[i - 1].idTableIndex].dataType != idtable[lextable[i + 1].idTableIndex].dataType)
				throw ERROR_THROW_IN(706, lextable[i].lineNumber, -1);

			//idtable[lextable[i].idTableIndex].value = idtable[lextable[i + 1].idTableIndex].value;
		}
	}
}

void TTM::SemanticAnalyzer::checkAccessingOutOfScopeId()
{
}

void TTM::SemanticAnalyzer::checkArgumentsMismatch()
{
}