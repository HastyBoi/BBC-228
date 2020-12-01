#include "pch.h"
#include "LexicalAnalyzer.h"
#include "FST.h"

char TTM::Tokenize(std::string_view str) {
	FST::FST nanomachinesSon[] = {
		FST_I32, FST_STR, FST_FN, FST_PARSE_INT, FST_IF, FST_ELSE, FST_CONCAT, FST_LET,
		FST_RETURN, FST_ECHO, FST_MAIN,
		FST_OPENING_PARENTHESIS, FST_CLOSING_PARENTHESIS, FST_SEMICOLON, FST_COMMA,
		FST_OPENING_CURLY_BRACE, FST_CLOSING_CURLY_BRACE,
		FST_PLUS, FST_MINUS, FST_ASTERISK, FST_SLASH, FST_PERCENT, FST_ASSIGN,
		FST_EQUALS, FST_NOT_EQUALS, FST_LESS, FST_GREATER, FST_LESS_OR_EQUALS, FST_GREATER_OR_EQUALS,
		FST_ID, FST_STRING_LITERAL, FST_INTEGER_LITERAL
	};
	const int size = sizeof(nanomachinesSon) / sizeof(nanomachinesSon[0]);
	const char tokens[] = {
		LEX_I32, LEX_STR, LEX_FN, LEX_PARSE_INT, LEX_IF, LEX_ELSE, LEX_CONCAT, LEX_LET,
		LEX_RET, LEX_ECHO, LEX_MAIN,
		LEX_OPENING_PARENTHESIS, LEX_CLOSING_PARENTHESIS, LEX_SEMICOLON, LEX_COMMA,
		LEX_OPENING_CURLY_BRACE, LEX_CLOSING_CURLY_BRACE,
		LEX_PLUS, LEX_MINUS, LEX_ASTERISK, LEX_SLASH, LEX_PERCENT, LEX_ASSIGN,
		LEX_EQUALS, LEX_NOT_EQUALS, LEX_LESS, LEX_GREATER, LEX_LESS_OR_EQUALS, LEX_GREATER_OR_EQUALS,
		LEX_ID, LEX_STRING_LITERAL, LEX_INTEGER_LITERAL
	};

	for (int i = 0; i < size; ++i) {
		if (execute(str, nanomachinesSon[i])) {
			return tokens[i];
		}
	}

	return EOF;
}

void TTM::Scan(LexTable& lextable, IdTable& idtable, const InputFileReader& in, Logger& log)
{
	auto splitted = InputFileReader::splitStringByDelimiter(in.fileText(), in::delimiter);
	for (std::string_view s : splitted)
	{
		char token = Tokenize(s);
		std::cout << token << '\n';
	}
}