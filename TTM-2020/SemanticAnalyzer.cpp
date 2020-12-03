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