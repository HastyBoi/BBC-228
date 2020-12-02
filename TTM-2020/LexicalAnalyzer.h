#pragma once
#include "Logger.h"
#include "LexTable.h"
#include "IdTable.h"

namespace TTM
{
	class LexicalAnalyzer
	{
	public:
		LexicalAnalyzer(const LexTable& lextable, const IdTable& idtable);

		void Scan(const std::vector<std::string>& sourceCode, Logger& log);
	private:
		LexTable lextable;
		IdTable idtable;

		char Tokenize(const std::string& str);
	};
}