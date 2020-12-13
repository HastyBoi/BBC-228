#include "pch.h"
#include "SemanticAnalyzer.h"
#include "Error.h"

TTM::SemanticAnalyzer::SemanticAnalyzer(LexTable& lextable, IdTable& idtable)
	: lextable(lextable), idtable(idtable)
{
	getAllFunctionParametersList();
}

void TTM::SemanticAnalyzer::Start(Logger& log)
{
	checkReturnTypeMismatch();
	checkArithmeticOperations();
	checkAssignmentTypeMismatch();
	checkParametersMismatch();

	log << "—емантический анализ выполнен без ошибок\n";
}

void TTM::SemanticAnalyzer::getAllFunctionParametersList()
{
	for (int i = 0; i < idtable.size(); ++i)
	{
		if (idtable[i].idType == it::id_type::function)
		{
			int index = idtable[idtable.getIdIndexByName("", idtable[i].name)].lexTableIndex + 1;
			functionParametersList[idtable[i].name] = getFunctionParametersList(index);
		}
	}
}

std::vector<TTM::it::data_type> TTM::SemanticAnalyzer::getFunctionParametersList(int startIndex)
{
	std::vector<it::data_type> argumentsList;
	for (int i = startIndex; lextable[i].lexeme != LEX_CLOSING_PARENTHESIS; ++i)
	{
		if (lextable[i].lexeme == LEX_ID && idtable[lextable[i].idTableIndex].idType != it::id_type::function
			|| lextable[i].lexeme == LEX_LITERAL)
		{
			argumentsList.push_back(idtable[lextable[i].idTableIndex].dataType);
		}
	}

	return argumentsList;
}

void TTM::SemanticAnalyzer::checkAssignmentTypeMismatch()
{
	for (int i = 0; i < lextable.size(); ++i)
	{
		if (lextable[i].lexeme == LEX_ASSIGN)
		{
			if (getPreviousOperandDataType(i - 1) != getNextOperandDataType(i + 1))
			{
				throw ERROR_THROW_LEX(706, lextable[i].lineNumber);
			}
		}
	}
}

void TTM::SemanticAnalyzer::checkArithmeticOperations()
{
	for (int i = 0; i < lextable.size(); ++i)
	{
		if (lextable[i].lexeme == LEX_PLUS || lextable[i].lexeme == LEX_MINUS
			|| lextable[i].lexeme == LEX_ASTERISK || lextable[i].lexeme == LEX_SLASH
			|| lextable[i].lexeme == LEX_PERCENT)
		{
			if (getPreviousOperandDataType(i - 1) != it::data_type::i32 || getNextOperandDataType(i + 1) != it::data_type::i32)
				throw ERROR_THROW_LEX(707, lextable[i].lineNumber);
		}
	}
}

void TTM::SemanticAnalyzer::checkReturnTypeMismatch()
{
	for (int i = 0; i < lextable.size(); ++i)
	{
		if (lextable[i].lexeme == LEX_RET
			&& getNextOperandDataType(i + 1) != getFunctionReturnTypeFromDeclaration(i))
		{
			throw ERROR_THROW_LEX(700, lextable[i].lineNumber);
		}
	}
}

void TTM::SemanticAnalyzer::checkParametersMismatch()
{
	for (int i = 0; i < lextable.size(); ++i)
	{
		if (lextable[i].lexeme == LEX_ASSIGN || lextable[i].lexeme == LEX_RET)
		{
			parametersMismatch(i + 1);
		}
	}
}

void TTM::SemanticAnalyzer::parametersMismatch(int startIndex)
{
	for (int i = startIndex; i < lextable.size() && lextable[i].lexeme != LEX_SEMICOLON; ++i)
	{
		if (lextable[i].lexeme == LEX_ID && idtable[lextable[i].idTableIndex].idType == it::id_type::function)
		{
			const std::string& functionName = idtable[lextable[i].idTableIndex].name;
			if (functionParametersList[functionName] != getFunctionParametersList(i))
			{
				throw ERROR_THROW_LEX(701, lextable[i].lineNumber);
			}
		}
	}
}

TTM::it::data_type TTM::SemanticAnalyzer::getNextOperandDataType(int startIndex) {
	for (int i = startIndex; i < lextable.size(); ++i)
	{
		if (lextable[i].lexeme == LEX_ID || lextable[i].lexeme == LEX_LITERAL)
		{
			return idtable[lextable[i].idTableIndex].dataType;
		}
	}

	return it::data_type::undefined;
}

TTM::it::data_type TTM::SemanticAnalyzer::getPreviousOperandDataType(int startIndex) {
	bool foundParenthesis = false;
	for (int i = startIndex; i > 0; --i)
	{
		if (lextable[i].lexeme == LEX_CLOSING_PARENTHESIS
			&& getFunctionDataType(i - 1) != it::data_type::undefined)
		{
			return getFunctionDataType(i - 1);
		}
		else if (lextable[i].lexeme == LEX_ID || lextable[i].lexeme == LEX_LITERAL)
		{
			return idtable[lextable[i].idTableIndex].dataType;
		}
	}

	return it::data_type::undefined;
}

TTM::it::data_type TTM::SemanticAnalyzer::getFunctionReturnTypeFromDeclaration(int startIndex)
{
	for (int i = startIndex; i > 0; --i)
	{
		if (lextable[i].lexeme == LEX_FN)
		{
			return idtable[lextable[i + 2].idTableIndex].dataType;
		}
	}

	return it::data_type::undefined;
}

TTM::it::data_type TTM::SemanticAnalyzer::getFunctionDataType(int startIndex)
{
	for (int i = startIndex; i > 0; --i)
	{
		int previousElementIndex = lextable[i - 1].idTableIndex;
		if (lextable[i].lexeme == LEX_OPENING_PARENTHESIS && previousElementIndex != TI_NULLIDX
			&& idtable[previousElementIndex].idType == it::id_type::function)
		{
			return idtable[previousElementIndex].dataType;
		}
	}
	return it::data_type::undefined;
}