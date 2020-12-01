#pragma once
#include "IdTable.h"
#include "LexTable.h"
#include "CommandLineArgumentsParser.h"
#include "Logger.h"

namespace TTM
{
	char Tokenize(std::string_view str);

	void Scan(LexTable& lextable, IdTable& idtable, const InputFileReader& in, Logger& log);
}