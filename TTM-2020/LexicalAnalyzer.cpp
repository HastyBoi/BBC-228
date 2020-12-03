#include "pch.h"
#include "LexicalAnalyzer.h"
#include "FST.h"

TTM::LexicalAnalyzer::LexicalAnalyzer(LexTable& lextable, IdTable& idtable)
	: lextable(lextable), idtable(idtable)
{	}

char TTM::LexicalAnalyzer::Tokenize(const std::string& str)
{
	FST::FST nanomachinesSon[] = {
		FST_I32, FST_STR, FST_FN, FST_PARSE_INT, FST_IF, FST_ELSE, FST_CONCAT, FST_LET,
		FST_RET, FST_ECHO, FST_MAIN,
		FST_OPENING_PARENTHESIS, FST_CLOSING_PARENTHESIS, FST_SEMICOLON, FST_COMMA,
		FST_OPENING_CURLY_BRACE, FST_CLOSING_CURLY_BRACE,
		FST_PLUS, FST_MINUS, FST_ASTERISK, FST_SLASH, FST_PERCENT,
		FST_EQUALS, FST_NOT_EQUALS, FST_LESS_OR_EQUALS, FST_GREATER_OR_EQUALS,
		FST_ASSIGN, FST_LESS, FST_GREATER,
		FST_ID, FST_STRING_LITERAL, FST_INTEGER_LITERAL
	};
	const int size = sizeof(nanomachinesSon) / sizeof(nanomachinesSon[0]);
	const char tokens[] = {
		LEX_I32, LEX_STR, LEX_FN, LEX_PARSE_INT, LEX_IF, LEX_ELSE, LEX_CONCAT, LEX_LET,
		LEX_RET, LEX_ECHO, LEX_MAIN,
		LEX_OPENING_PARENTHESIS, LEX_CLOSING_PARENTHESIS, LEX_SEMICOLON, LEX_COMMA,
		LEX_OPENING_CURLY_BRACE, LEX_CLOSING_CURLY_BRACE,
		LEX_PLUS, LEX_MINUS, LEX_ASTERISK, LEX_SLASH, LEX_PERCENT,
		LEX_EQUALS, LEX_NOT_EQUALS, LEX_LESS_OR_EQUALS, LEX_GREATER_OR_EQUALS,
		LEX_ASSIGN, LEX_LESS, LEX_GREATER,
		LEX_ID, LEX_STRING_LITERAL, LEX_INTEGER_LITERAL
	};

	for (int i = 0; i < size; ++i) {
		if (execute(str, nanomachinesSon[i])) {
			return tokens[i];
		}
	}

	return EOF;
}

void TTM::LexicalAnalyzer::Scan(const std::vector<std::pair<std::string, int>>& sourceCode, Logger& log)
{
	using type = it::data_type;
	using id_t = it::id_type;

	std::string currentScope = "";
	std::string previousScope = currentScope;
	std::string lastFunctionName = "";
	id_t idType;

	for (size_t i = 0; i < sourceCode.size(); ++i)
	{
		char token = Tokenize(sourceCode[i].first);
		if (token == EOF)
		{
			//todo add new error type
			//std::cerr << sourceCode[i].second << " " << sourceCode[i].first << '\n';
			throw ERROR_THROW_IN(129, sourceCode[i].second, -1);
		}

		int idTableIndex = TI_NULLIDX;

		switch (token)
		{
		case LEX_CONCAT:
			idTableIndex = idtable.getIdIndexByName("", sourceCode[i].first, id_t::function);
			if (idTableIndex == TI_NULLIDX)
			{
				idTableIndex = idtable.addEntry({ sourceCode[i].first, "", lextable.size(), type::str, id_t::function, "" });
			}
			break;

		case LEX_PARSE_INT:
			idTableIndex = idtable.getIdIndexByName("", sourceCode[i].first, id_t::function);
			if (idTableIndex == TI_NULLIDX)
			{
				idTableIndex = idtable.addEntry({ sourceCode[i].first, "", lextable.size(), type::i32, id_t::function, "" });
			}
			break;

		case LEX_ID:
		case LEX_MAIN:
			if (idTableIndex == TI_NULLIDX)
			{
				if (lextable.declaredFunction())
				{
					lastFunctionName = sourceCode[i].first;
					currentScope = "";
					idType = id_t::function;
					idTableIndex = idtable.addEntry({ sourceCode[i].first, currentScope, lextable.size(), lextable.getDatatypeFromDeclaration(), idType, "" });
				}
				else if (lextable.declaredVariable())
				{
					idType = id_t::variable;
					idTableIndex = idtable.addEntry({ sourceCode[i].first, currentScope, lextable.size(), lextable.getDatatypeFromDeclaration(), idType, "" });
				}
				else if (lextable.declaredDatatype())
				{
					idType = id_t::parameter;
					idTableIndex = idtable.addEntry({ sourceCode[i].first, currentScope, lextable.size(), lextable.getDatatypeFromDeclaration(), idType, "" });
				}
			}
			break;

		case LEX_INTEGER_LITERAL:
			idTableIndex = idtable.getLiteralIndexByValue(atoi(sourceCode[i].first.c_str()));
			if (idTableIndex == TI_NULLIDX)
			{
				idTableIndex = idtable.addEntry({ "L" + std::to_string(lextable.size()), "", lextable.size(), type::i32, id_t::literal, sourceCode[i].first.c_str() });
			}
			token = LEX_LITERAL;
			break;

		case LEX_STRING_LITERAL:
			idTableIndex = idtable.getLiteralIndexByValue(sourceCode[i].first.c_str());
			if (idTableIndex == TI_NULLIDX)
			{
				idTableIndex = idtable.addEntry({ "L" + std::to_string(lextable.size()), "", lextable.size(), type::str, id_t::literal, sourceCode[i].first.c_str() });
			}
			token = LEX_LITERAL;
			break;

		case LEX_OPENING_CURLY_BRACE:
			previousScope = currentScope;
			currentScope = lastFunctionName;
			break;

		case LEX_CLOSING_CURLY_BRACE:
			currentScope = previousScope;
			break;

		case LEX_OPENING_PARENTHESIS:
			previousScope = currentScope;
			currentScope = lastFunctionName;
			break;

		case LEX_CLOSING_PARENTHESIS:
			currentScope = previousScope;
			break;

		default:
			break;
		}

		if (token == LEX_I32 || token == LEX_STR)
			token = LEX_DATATYPE;

		lextable.addEntry(LexTable::Entry(token, sourceCode[i].second, idTableIndex));
	}

	log << "Лексический анализ выполнен без ошибок\n";
}