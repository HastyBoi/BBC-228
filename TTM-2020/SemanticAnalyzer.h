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

		using list_t = std::map<std::string, std::vector<it::data_type>>;

		list_t functionParametersList;

		void getAllFunctionParametersList();
		std::vector<it::data_type> getFunctionParametersList(int startIndex);

		void parametersMismatch(int startIndex);

		void checkAssignmentTypeMismatch();
		void checkArithmeticOperations();
		void checkReturnTypeMismatch();
		void checkParametersMismatch();

		it::data_type getNextOperandDataType(int startIndex);
		it::data_type getPreviousOperandDataType(int startIndex);
		it::data_type getFunctionReturnTypeFromDeclaration(int startIndex);
		it::data_type getFunctionDataType(int startIndex);
	};
}