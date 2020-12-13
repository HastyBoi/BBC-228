#pragma once
#include "Logger.h"
#include "LexTable.h"
#include "IdTable.h"

namespace TTM
{
	class LexicalAnalyzer
	{
	public:
		LexicalAnalyzer(LexTable& lextable, IdTable& idtable);

		void Scan(const std::vector<std::pair<std::string, int>>& sourceCode, Logger& log);
	private:
		LexTable& lextable;
		IdTable& idtable;

		char tokenize(const std::string& str);
		void includeStdlibFunctions();
	};
}