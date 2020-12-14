#include "pch.h"
#include "PolishNotation.h"

TTM::PolishNotation::PolishNotation(LexTable& lextable, IdTable& idtable)
	:lextable(lextable), idtable(idtable)
{
	for (int i = 0; i < lextable.size(); ++i)
	{
		if (lextable[i].lexeme == LEX_ASSIGN || lextable[i].lexeme == LEX_RET)
		{
			convert(i + 1);
		}
	}
}

int TTM::PolishNotation::getOperationsPriority(char operation)
{
	if (operation == LEX_OPENING_PARENTHESIS || operation == LEX_CLOSING_PARENTHESIS)
		return 1;
	else if (operation == LEX_PLUS || operation == LEX_MINUS)
		return 2;
	else if (operation == LEX_ASTERISK || operation == LEX_SLASH || operation == LEX_PERCENT)
		return 3;
	return EOF;
}

bool TTM::PolishNotation::convert(int startIndex)
{
	std::vector<LexTable::Entry> infixExpressionEntries;
	int operandsCounter = 0, operationsCounter = 0;
	bool functionParameters = false;

	for (int i = startIndex; i < lextable.size() && lextable[i - 1].lexeme != LEX_SEMICOLON; ++i)
	{
		char& lexeme = lextable[i].lexeme;
		if (lextable[i].idTableIndex != TI_NULLIDX && idtable[lextable[i].idTableIndex].idType == it::id_type::function)
		{
			lexeme = LEX_FUNCTION_CALL;
			functionParameters = true;
			++operandsCounter;
		}
		else if (lexeme == LEX_CLOSING_PARENTHESIS && functionParameters)
		{
			functionParameters = false;
		}
		else if (lexeme == LEX_PLUS || lexeme == LEX_MINUS || lexeme == LEX_ASTERISK || lexeme == LEX_SLASH || lexeme == LEX_PERCENT)
		{
			++operationsCounter;
		}
		else if (!functionParameters && (lexeme == LEX_ID || lexeme == LEX_LITERAL))
		{
			++operandsCounter;
		}

		infixExpressionEntries.push_back(lextable[i]);
	}

	if (operandsCounter - operationsCounter != 1)
		return false;

	std::vector<LexTable::Entry> postfixExpressionEntries = getPostfixExpression(infixExpressionEntries);
	for (size_t i = 0; i < infixExpressionEntries.size(); ++i) {
		if (i < postfixExpressionEntries.size()) {
			lextable[i + startIndex] = postfixExpressionEntries[i];
		}
		else {
			lextable[i + startIndex] = { FORBIDDEN_SYMBOL, EOF, EOF };
		}
	}

	return true;
}

std::vector<TTM::LexTable::Entry> TTM::PolishNotation::getPostfixExpression(const std::vector<LexTable::Entry>& entries)
{
	std::vector<LexTable::Entry> output;
	output.reserve(entries.size());
	std::stack<LexTable::Entry> stack;
	bool foundFunction = false;

	for (const auto& e : entries)
	{
		if (e.lexeme == LEX_PLUS || e.lexeme == LEX_MINUS || e.lexeme == LEX_ASTERISK
			|| e.lexeme == LEX_SLASH || e.lexeme == LEX_PERCENT) {
			if (!stack.empty() && stack.top().lexeme != LEX_OPENING_PARENTHESIS) {
				while (!stack.empty() && getOperationsPriority(e.lexeme) <= getOperationsPriority(stack.top().lexeme))
				{
					output.push_back(stack.top());
					stack.pop();
				}
			}

			stack.push(e);
		}
		else if (e.lexeme == LEX_COMMA)
		{
			while (!stack.empty() && stack.top().lexeme != LEX_OPENING_PARENTHESIS)
			{
				output.push_back(stack.top());
				stack.pop();
			}
		}
		else if (e.lexeme == LEX_FUNCTION_CALL)
		{
			foundFunction = true;
			stack.push(e);
		}
		else if (e.lexeme == LEX_OPENING_PARENTHESIS)
		{
			stack.push(e);
		}
		else if (e.lexeme == LEX_CLOSING_PARENTHESIS)
		{
			while (stack.top().lexeme != LEX_OPENING_PARENTHESIS)
			{
				output.push_back(stack.top());
				stack.pop();
			}
			stack.pop();

			if (!stack.empty() && stack.top().lexeme == LEX_FUNCTION_CALL)
			{
				output.push_back(stack.top());
				output.push_back({ getFunctionParametersCountByName(idtable[stack.top().idTableIndex].name), TI_NULLIDX, TI_NULLIDX });
				stack.pop();
				foundFunction = false;
			}
		}
		else if (e.lexeme != LEX_SEMICOLON)
		{
			output.push_back(e);
		}
	}

	while (!stack.empty())
	{
		output.push_back(stack.top());
		stack.pop();
	}

	output.push_back(entries.back());

	return output;
}

char TTM::PolishNotation::getFunctionParametersCountByName(const std::string& functionName)
{
	int parametersCount = 0;
	int index = idtable.getIdIndexByName("", functionName);
	for (int i = index + 1; idtable[i].idType == it::id_type::parameter; ++i)
	{
		++parametersCount;
	}

	return parametersCount + '0';
}