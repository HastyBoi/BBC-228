#include "pch.h"
#include "PolishNotation.h"

TTM::PolishNotation::PolishNotation(LexTable& lextable, IdTable& idtable)
	:lextable(lextable), idtable(idtable)
{
	for (int i = 0; i < lextable.size(); ++i)
	{
		if (lextable[i].lexeme == LEX_ASSIGN)
		{
			shokk(i);
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

bool TTM::PolishNotation::shokk(int startIndex)
{
	std::vector<LexTable::Entry> infixExpressionEntries;
	int operandsCounter = 0, operationsCounter = 0;

	for (int i = startIndex + 1; i < lextable.size() && lextable[i - 1].lexeme != LEX_SEMICOLON; ++i)
	{
		char& lexeme = lextable[i].lexeme;
		if (lextable[i].idTableIndex != TI_NULLIDX && idtable[lextable[i].idTableIndex].idType == it::id_type::function)
		{
			lexeme = LEX_FUNCTION_CALL;
		}
		else if (lexeme == LEX_COMMA)
		{
			--operandsCounter;
		}
		else if (lexeme == LEX_PLUS || lexeme == LEX_MINUS || lexeme == LEX_ASTERISK || lexeme == LEX_SLASH || lexeme == LEX_PERCENT)
		{
			++operationsCounter;
		}
		else if (lexeme == LEX_ID || lexeme == LEX_LITERAL)
		{
			++operandsCounter;
		}

		infixExpressionEntries.push_back(lextable[i]);
	}

	if (operandsCounter - operationsCounter != 1)
		return false;

	std::vector<LexTable::Entry> postfixExpressionEntries = convert(infixExpressionEntries);
	for (size_t i = 0; i < infixExpressionEntries.size(); ++i) {
		if (i < postfixExpressionEntries.size()) {
			lextable[i + startIndex + 1] = postfixExpressionEntries[i];
		}
		else {
			lextable[i + startIndex + 1] = { FORBIDDEN_SYMBOL, EOF, EOF };
		}
	}

	return true;
}

std::vector<TTM::LexTable::Entry> TTM::PolishNotation::convert(const std::vector<LexTable::Entry>& entries)
{
	std::vector<LexTable::Entry> output;
	output.reserve(entries.size());
	std::stack<LexTable::Entry> stack;
	bool foundFunction = false;
	int parametersCounter = 0;

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
				stack.pop();
				output.push_back({ static_cast<char>(parametersCounter + '0'), TI_NULLIDX, TI_NULLIDX });
				foundFunction = false;
				parametersCounter = 0;
			}
		}
		else if (e.lexeme != LEX_SEMICOLON)
		{
			if (foundFunction && e.lexeme == LEX_ID || e.lexeme == LEX_LITERAL)
			{
				++parametersCounter;
			}

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