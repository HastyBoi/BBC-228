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
		void checkAccessingOutOfScopeId();
		void checkArgumentsMismatch();
	};
}