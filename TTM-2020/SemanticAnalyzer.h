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

		std::map<std::string, std::vector<it::data_type>> functionParameters;

		void checkAssignmentTypeMismatch();
		void checkReturnTypeMismatch();
		void checkArgumentsAmountMismatch();

		void getAllFunctionParameters();
	};
}