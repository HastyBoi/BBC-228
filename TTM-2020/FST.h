#pragma once
#include "In.h"
#include "LT.h"
#include "IT.h"
#include "Log.h"
#include "Parm.h"

#pragma region FST

#pragma region I32

#define FST_I32 FST::FST(4,\
FST::NODE(1, FST::RELATION('i', 1)),\
FST::NODE(1, FST::RELATION('3', 2)),\
FST::NODE(1, FST::RELATION('2', 3)),\
FST::NODE())

#pragma endregion

#pragma region STR

#define FST_STR FST::FST(4,\
FST::NODE(1, FST::RELATION('s', 1)),\
FST::NODE(1, FST::RELATION('t', 2)),\
FST::NODE(1, FST::RELATION('r', 3)),\
FST::NODE())

#pragma endregion

#pragma region FN

#define FST_FN FST::FST(3,\
FST::NODE(1, FST::RELATION('f', 1)),\
FST::NODE(1, FST::RELATION('n', 2)),\
FST::NODE())

#pragma endregion

#pragma region PARSE_INT

#define FST_PARSE_INT FST::FST(9,\
FST::NODE(1, FST::RELATION('p', 1)),\
FST::NODE(1, FST::RELATION('a', 2)),\
FST::NODE(1, FST::RELATION('r', 3)),\
FST::NODE(1, FST::RELATION('s', 4)),\
FST::NODE(1, FST::RELATION('e', 5)),\
FST::NODE(1, FST::RELATION('I', 6)),\
FST::NODE(1, FST::RELATION('n', 7)),\
FST::NODE(1, FST::RELATION('t', 8)),\
FST::NODE())

#pragma endregion

#pragma region CONCAT

#define FST_CONCAT FST::FST(7,\
FST::NODE(1, FST::RELATION('c', 1)),\
FST::NODE(1, FST::RELATION('o', 2)),\
FST::NODE(1, FST::RELATION('n', 3)),\
FST::NODE(1, FST::RELATION('c', 4)),\
FST::NODE(1, FST::RELATION('a', 5)),\
FST::NODE(1, FST::RELATION('t', 6)),\
FST::NODE())

#pragma endregion

#pragma region IF

#define FST_IF FST::FST(3,\
FST::NODE(1, FST::RELATION('i', 1)),\
FST::NODE(1, FST::RELATION('f', 2)),\
FST::NODE())

#pragma endregion

#pragma region ELSE

#define FST_ELSE FST::FST(5,\
FST::NODE(1, FST::RELATION('e', 1)),\
FST::NODE(1, FST::RELATION('l', 2)),\
FST::NODE(1, FST::RELATION('s', 3)),\
FST::NODE(1, FST::RELATION('e', 4)),\
FST::NODE())

#pragma endregion

#pragma region LET

#define FST_LET FST::FST(4,\
FST::NODE(1, FST::RELATION('l', 1)),\
FST::NODE(1, FST::RELATION('e', 2)),\
FST::NODE(1, FST::RELATION('t', 3)),\
FST::NODE())

#pragma endregion

#pragma region RET

#define FST_RETURN FST::FST(4,\
FST::NODE(1, FST::RELATION('r', 1)),\
FST::NODE(1, FST::RELATION('e', 2)),\
FST::NODE(1, FST::RELATION('t', 3)),\
FST::NODE())

#pragma endregion

#pragma region ECHO

#define FST_ECHO FST::FST(5,\
FST::NODE(1, FST::RELATION('e', 1)),\
FST::NODE(1, FST::RELATION('c', 2)),\
FST::NODE(1, FST::RELATION('h', 3)),\
FST::NODE(1, FST::RELATION('o', 4)),\
FST::NODE())

#pragma endregion

#pragma region MAIN

#define FST_MAIN FST::FST(5,\
FST::NODE(1, FST::RELATION('m', 1)),\
FST::NODE(1, FST::RELATION('a', 2)),\
FST::NODE(1, FST::RELATION('i', 3)),\
FST::NODE(1, FST::RELATION('n', 4)),\
FST::NODE())

#pragma endregion

#pragma region LEFTHESIS

#define FST_OPENING_PARENTHESIS FST::FST(2,\
FST::NODE(1, FST::RELATION('(', 1)),\
FST::NODE())

#pragma endregion

#pragma region RIGHTHESIS

#define FST_CLOSING_PARENTHESIS FST::FST(2,\
FST::NODE(1, FST::RELATION(')', 1)),\
FST::NODE())

#pragma endregion

#pragma region SEMICOLON

#define FST_SEMICOLON FST::FST(2,\
FST::NODE(1, FST::RELATION(';', 1)),\
FST::NODE())

#pragma endregion

#pragma region COMMA

#define FST_COMMA FST::FST(2,\
FST::NODE(1, FST::RELATION(',', 1)),\
FST::NODE())

#pragma endregion

#pragma region OPENING_CURLY_BRACE

#define FST_OPENING_CURLY_BRACE FST::FST(2,\
FST::NODE(1, FST::RELATION('{', 1)),\
FST::NODE())

#pragma endregion

#pragma region CLOSING_CURLY_BRACE

#define FST_CLOSING_CURLY_BRACE FST::FST(2,\
FST::NODE(1, FST::RELATION('}', 1)),\
FST::NODE())

#pragma endregion

#pragma region PLUS

#define FST_PLUS FST::FST(2,\
FST::NODE(1, FST::RELATION('+', 1)),\
FST::NODE())

#pragma endregion

#pragma region MINUS

#define FST_MINUS FST::FST(2,\
FST::NODE(1, FST::RELATION('-', 1)),\
FST::NODE())

#pragma endregion

#pragma region ASTERISK

#define FST_ASTERISK FST::FST(2,\
FST::NODE(1, FST::RELATION('*', 1)),\
FST::NODE())

#pragma endregion

#pragma region SLASH

#define FST_SLASH FST::FST(2,\
FST::NODE(1, FST::RELATION('/', 1)),\
FST::NODE())

#pragma endregion

#pragma region PERCENT

#define FST_PERCENT FST::FST(2,\
FST::NODE(1, FST::RELATION('%', 1)),\
FST::NODE())

#pragma endregion

#pragma region ASSIGN

#define FST_ASSIGN FST::FST(2,\
FST::NODE(1, FST::RELATION('=', 1)),\
FST::NODE())

#pragma endregion

#pragma region IDENTIFIER

#define FST_ID FST::FST(2, \
FST::NODE(116,\
FST::RELATION('a', 0), FST::RELATION('a', 1),\
FST::RELATION('b', 0), FST::RELATION('b', 1),\
FST::RELATION('c', 0), FST::RELATION('c', 1),\
FST::RELATION('d', 0), FST::RELATION('d', 1),\
FST::RELATION('e', 0), FST::RELATION('e', 1),\
FST::RELATION('f', 0), FST::RELATION('f', 1),\
FST::RELATION('g', 0), FST::RELATION('g', 1),\
FST::RELATION('h', 0), FST::RELATION('h', 1),\
FST::RELATION('i', 0), FST::RELATION('i', 1),\
FST::RELATION('j', 0), FST::RELATION('j', 1),\
FST::RELATION('k', 0), FST::RELATION('k', 1),\
FST::RELATION('l', 0), FST::RELATION('l', 1),\
FST::RELATION('m', 0), FST::RELATION('m', 1),\
FST::RELATION('n', 0), FST::RELATION('n', 1),\
FST::RELATION('o', 0), FST::RELATION('o', 1),\
FST::RELATION('p', 0), FST::RELATION('p', 1),\
FST::RELATION('q', 0), FST::RELATION('q', 1),\
FST::RELATION('r', 0), FST::RELATION('r', 1),\
FST::RELATION('s', 0), FST::RELATION('s', 1),\
FST::RELATION('t', 0), FST::RELATION('t', 1),\
FST::RELATION('u', 0), FST::RELATION('u', 1),\
FST::RELATION('v', 0), FST::RELATION('v', 1),\
FST::RELATION('w', 0), FST::RELATION('w', 1),\
FST::RELATION('x', 0), FST::RELATION('x', 1),\
FST::RELATION('y', 0), FST::RELATION('y', 1),\
FST::RELATION('z', 0), FST::RELATION('z', 1),\
FST::RELATION('а', 0), FST::RELATION('а', 1),\
FST::RELATION('б', 0), FST::RELATION('б', 1),\
FST::RELATION('в', 0), FST::RELATION('в', 1),\
FST::RELATION('г', 0), FST::RELATION('г', 1),\
FST::RELATION('д', 0), FST::RELATION('д', 1),\
FST::RELATION('е', 0), FST::RELATION('е', 1),\
FST::RELATION('ж', 0), FST::RELATION('ж', 1),\
FST::RELATION('з', 0), FST::RELATION('з', 1),\
FST::RELATION('и', 0), FST::RELATION('и', 1),\
FST::RELATION('й', 0), FST::RELATION('й', 1),\
FST::RELATION('к', 0), FST::RELATION('к', 1),\
FST::RELATION('л', 0), FST::RELATION('л', 1),\
FST::RELATION('м', 0), FST::RELATION('м', 1),\
FST::RELATION('н', 0), FST::RELATION('н', 1),\
FST::RELATION('о', 0), FST::RELATION('о', 1),\
FST::RELATION('п', 0), FST::RELATION('п', 1),\
FST::RELATION('р', 0), FST::RELATION('р', 1),\
FST::RELATION('с', 0), FST::RELATION('с', 1),\
FST::RELATION('т', 0), FST::RELATION('т', 1),\
FST::RELATION('у', 0), FST::RELATION('у', 1),\
FST::RELATION('ф', 0), FST::RELATION('ф', 1),\
FST::RELATION('х', 0), FST::RELATION('х', 1),\
FST::RELATION('ц', 0), FST::RELATION('ц', 1),\
FST::RELATION('ч', 0), FST::RELATION('ч', 1),\
FST::RELATION('ш', 0), FST::RELATION('ш', 1),\
FST::RELATION('щ', 0), FST::RELATION('щ', 1),\
FST::RELATION('ъ', 0), FST::RELATION('ъ', 1),\
FST::RELATION('ы', 0), FST::RELATION('ы', 1),\
FST::RELATION('ь', 0), FST::RELATION('ь', 1),\
FST::RELATION('э', 0), FST::RELATION('э', 1),\
FST::RELATION('ю', 0), FST::RELATION('ю', 1),\
FST::RELATION('я', 0), FST::RELATION('я', 1)\
),\
FST::NODE())

#pragma endregion

#pragma region STRING_LITERAL

#define FST_STRING_LITERAL FST::FST(3,\
FST::NODE(1, FST::RELATION('\'', 1)),\
FST::NODE(70,\
FST::RELATION('a', 1),\
FST::RELATION('b', 1),\
FST::RELATION('c', 1),\
FST::RELATION('d', 1),\
FST::RELATION('e', 1),\
FST::RELATION('f', 1),\
FST::RELATION('g', 1),\
FST::RELATION('h', 1),\
FST::RELATION('i', 1),\
FST::RELATION('j', 1),\
FST::RELATION('k', 1),\
FST::RELATION('l', 1),\
FST::RELATION('m', 1),\
FST::RELATION('n', 1),\
FST::RELATION('o', 1),\
FST::RELATION('p', 1),\
FST::RELATION('q', 1),\
FST::RELATION('r', 1),\
FST::RELATION('s', 1),\
FST::RELATION('t', 1),\
FST::RELATION('u', 1),\
FST::RELATION('v', 1),\
FST::RELATION('w', 1),\
FST::RELATION('x', 1),\
FST::RELATION('y', 1),\
FST::RELATION('z', 1),\
FST::RELATION('а', 1),\
FST::RELATION('б', 1),\
FST::RELATION('в', 1),\
FST::RELATION('г', 1),\
FST::RELATION('д', 1),\
FST::RELATION('е', 1),\
FST::RELATION('ж', 1),\
FST::RELATION('з', 1),\
FST::RELATION('и', 1),\
FST::RELATION('й', 1),\
FST::RELATION('к', 1),\
FST::RELATION('л', 1),\
FST::RELATION('м', 1),\
FST::RELATION('н', 1),\
FST::RELATION('о', 1),\
FST::RELATION('п', 1),\
FST::RELATION('р', 1),\
FST::RELATION('с', 1),\
FST::RELATION('т', 1),\
FST::RELATION('у', 1),\
FST::RELATION('ф', 1),\
FST::RELATION('х', 1),\
FST::RELATION('ц', 1),\
FST::RELATION('ч', 1),\
FST::RELATION('ш', 1),\
FST::RELATION('щ', 1),\
FST::RELATION('ъ', 1),\
FST::RELATION('ы', 1),\
FST::RELATION('ь', 1),\
FST::RELATION('э', 1),\
FST::RELATION('ю', 1),\
FST::RELATION('я', 1),\
FST::RELATION('0', 1),\
FST::RELATION('1', 1),\
FST::RELATION('2', 1),\
FST::RELATION('3', 1),\
FST::RELATION('4', 1),\
FST::RELATION('5', 1),\
FST::RELATION('6', 1),\
FST::RELATION('7', 1),\
FST::RELATION('8', 1),\
FST::RELATION('9', 1),\
FST::RELATION(' ', 1),\
FST::RELATION('\'', 2)\
),\
FST::NODE())

#pragma endregion

#pragma region INTEGER_LITERAL

#define FST_INTEGER_LITERAL FST::FST(2,\
FST::NODE(20,\
FST::RELATION('0', 0), FST::RELATION('0', 1),\
FST::RELATION('1', 0), FST::RELATION('1', 1),\
FST::RELATION('2', 0), FST::RELATION('2', 1),\
FST::RELATION('3', 0), FST::RELATION('3', 1),\
FST::RELATION('4', 0), FST::RELATION('4', 1),\
FST::RELATION('5', 0), FST::RELATION('5', 1),\
FST::RELATION('6', 0), FST::RELATION('6', 1),\
FST::RELATION('7', 0), FST::RELATION('7', 1),\
FST::RELATION('8', 0), FST::RELATION('8', 1),\
FST::RELATION('9', 0), FST::RELATION('9', 1)\
),\
FST::NODE())

#pragma endregion

#pragma endregion

namespace FST {
	struct RELATION {			// ребро:символ -> вершина графа переходов КА
		char symbol;			// символ перехода
		short nnode;			// номер смежной вершины

		RELATION(char c = 0x00, short ns = NULL);
	};

	struct NODE {				// вершина графа переходов
		short n_relation;		// количество инцидентных ребер
		RELATION* relations;	// инцидентные ребра

		NODE();
		NODE(short n, RELATION rel, ...);
	};

	struct FST {				// недетерминированный конечный автомат
		short position;			// текущая позиция в цепочке
		short nstates;			// количество состояний автомата
		NODE* nodes;			// граф переходов: [0] - начальное состояние, [nstate-1] - конечное
		short* rstates;			// возможные состояния автомата на данной позиции

		FST(short ns, NODE n, ...);
		~FST();
	};

	// выполнить распознавание цепочки
	bool execute(const char* string, FST& fst);
};