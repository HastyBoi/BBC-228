#pragma once
#define LT_MAXSIZE						4096
#define LT_TI_NULLIDX					((int)0xffffffff)
#define LEX_I32							'n'
#define LEX_STR							's'
#define LEX_DATATYPE					't'
#define LEX_ID							'i'
#define LEX_INTEGER_LITERAL				'1'
#define LEX_STRING_LITERAL				'2'
#define LEX_LITERAL						'l'
#define LEX_FN							'f'
#define LEX_PARSE_INT					'P'
#define LEX_CONCAT						'C'
#define LEX_IF							'I'
#define LEX_ELSE						'E'
#define LEX_LET							'd'
#define LEX_RET							'r'
#define LEX_ECHO						'p'
#define LEX_MAIN						'm'
#define LEX_SEMICOLON					';'
#define LEX_COMMA						','
#define LEX_OPENING_CURLY_BRACE			'{'
#define LEX_CLOSING_CURLY_BRACE			'}'
#define LEX_OPENING_PARENTHESIS			'('
#define LEX_CLOSING_PARENTHESIS			')'
#define LEX_PLUS						'+'
#define LEX_MINUS						'-'
#define LEX_ASTERISK					'*'
#define LEX_SLASH						'/'
#define LEX_PERCENT						'%'
#define LEX_ASSIGN						'='
#define LEX_EQUALS						'q'
#define LEX_NOT_EQUALS					'!'
#define LEX_LESS						'<'
#define LEX_GREATER						'>'
#define LEX_LESS_OR_EQUALS				'o'
#define LEX_GREATER_OR_EQUALS			'x'

namespace TTM {
	class LexTable
	{
	public:
		struct Entry
		{
			int lineNumber;
			int idxTI;
			std::string lexeme;
		};

		LexTable(size_t capacity = 0);
		void AddEntry(const Entry& entry);

		Entry& operator [](size_t index);

	private:
		std::vector<Entry> m_table;
		size_t m_size;
	};
}