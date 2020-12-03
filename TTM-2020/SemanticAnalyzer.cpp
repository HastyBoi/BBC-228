#include "pch.h"
#include "SemanticAnalyzer.h"
#include "Error.h"

TTM::SemanticAnalyzer::SemanticAnalyzer(LexTable& lextable, IdTable& idtable)
	: lextable(lextable), idtable(idtable)
{	}

void TTM::SemanticAnalyzer::Start(Logger& log)
{
	checkTypeMismatch();
	checkUndeclaredVariable();
	checkMultipleDeclaration();
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
		}
	}
}

void TTM::SemanticAnalyzer::checkUndeclaredVariable()
{
}

void TTM::SemanticAnalyzer::checkMultipleDeclaration()
{
}

void TTM::SemanticAnalyzer::checkAccessingOutOfScopeId()
{
}

void TTM::SemanticAnalyzer::checkArgumentsMismatch()
{
}