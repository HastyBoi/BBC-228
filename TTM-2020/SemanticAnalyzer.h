#pragma once
#include "LexTable.h"
#include "IdTable.h"
#include "Logger.h"

namespace TTM
{
	class SemanticAnalyzer
	{
	public:
		SemanticAnalyzer(LexTable& lextable, IdTable& idtable);
		void Start(Logger& log);
	private:
		LexTable& lextable;
		IdTable& idtable;

		void checkTypeMismatch();
		void checkUndeclaredVariable();
		void checkMultipleDeclaration();
		void checkAccessingOutOfScopeId();
		void checkArgumentsMismatch();
	};
}