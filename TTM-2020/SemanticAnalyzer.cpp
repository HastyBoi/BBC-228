#include "pch.h"
#include "SemanticAnalyzer.h"
#include "Error.h"

void SA::SemanticAnalyzer::CheckReturn()
{
	bool main = false;
	for (int i = 0; i < lextable.size; i++)
		if (lextable.table[i].lexeme == LEX_RET) {
			if (main) {
				if ((lextable.table[i + 1].lexeme == LEX_ID || lextable.table[i + 1].lexeme == LEX_LITERAL)
					&& idtable.table[lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::INT)
					throw ERROR_THROW_IN(708, lextable.table[i].lineNumber, 0);
			}
			else if (lextable.table[i + 1].lexeme == LEX_ID) {
				if (idtable.table[lextable.table[i + 1].idxTI].idtype == IT::IDTYPE::F)
					throw ERROR_THROW_IN(700, lextable.table[i].lineNumber, 0);
			}
		}
		else if (lextable.table[i].lexeme == LEX_MAIN)
			main = true;
}

void SA::SemanticAnalyzer::ParamsOfFunc()
{
	char buf[ID_MAXSIZE];
	int par = 0, funcpar = 0;
	for (int i = 0; i < lextable.size; i++)
		if (lextable.table[i].lexeme == LEX_ASSIGN) {
			while (lextable.table[i].lexeme == LEX_SEMICOLON) {
				i++;
				if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F) {
					strcpy_s(buf, idtable.table[lextable.table[i].idxTI].id);
					for (int j = 0; j < i; j++)
						if (lextable.table[j].lexeme == LEX_ID && lextable.table[j - 2].lexeme == LEX_FN
							&& !strcmp(buf, idtable.table[lextable.table[j].idxTI].id)) {
							j++;
							for (j; lextable.table[j].lexeme != LEX_CLOSING_PARENTHESIS; j++)
								if (lextable.table[j].lexeme == LEX_ID) {
									funcpar++;
									i += 2;
									if (idtable.table[lextable.table[j].idxTI].iddatatype == IT::IDDATATYPE::INT) {
										if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L
											&& idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT)
											par++;
										if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT)
											par++;
									}
									if (idtable.table[lextable.table[j].idxTI].iddatatype == IT::IDDATATYPE::STR) {
										if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L
											&& idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
											par++;
										if (lextable.table[i].lexeme == LEX_ID)
											if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
												par++;
									}
								}
							if (funcpar != par || lextable.table[i + 2].lexeme != LEX_SEMICOLON)
								throw ERROR_THROW_IN(701, lextable.table[i].lineNumber, 0);
						}
				}
			}
		}
}

void SA::SemanticAnalyzer::ParamsOfStdFunc()
{
	for (int i = 0; i < lextable.size; i++)
		switch (lextable.table[i].lexeme) {
		case LEX_ECHO: {
			if (lextable.table[i + 1].lexeme != LEX_ID && lextable.table[i + 1].lexeme != LEX_LITERAL)
				throw ERROR_THROW_IN(704, lextable.table[i].lineNumber, 0);
			break;
		}
		case LEX_PARSE_INT:
		case LEX_CONCAT: {
			int parmsCount = 0;
			while (lextable.table[i].lexeme != LEX_SEMICOLON) {
				i++;
				if (lextable.table[i].lexeme == LEX_ID) {
					parmsCount++;
					if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::STR)
						throw ERROR_THROW_IN(702, lextable.table[i].lineNumber, 0);
				}
			}
			if (parmsCount != 2)
				throw ERROR_THROW_IN(703, lextable.table[i].lineNumber, 0);
			break;
		}
		}
}

void SA::SemanticAnalyzer::Types()
{
	IT::IDDATATYPE datatype;
	for (int i = 0; i < lextable.size; i++) {
		if (lextable.table[i].lexeme == LEX_ASSIGN) {
			datatype = idtable.table[lextable.table[i - 1].idxTI].iddatatype;
			while (lextable.table[i].lexeme != LEX_SEMICOLON) {
				i++;
				if ((lextable.table[i].lexeme == LEX_ID || lextable.table[i].lexeme == LEX_LITERAL)
					&& idtable.table[lextable.table[i].idxTI].iddatatype != datatype) {
					if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F)
					{
						throw ERROR_THROW_IN(705, lextable.table[i].lineNumber, 0);
					}
					else
					{
						throw ERROR_THROW_IN(706, lextable.table[i].lineNumber, 0);
					}
				}
				else if (lextable.table[i].lexeme == LEX_ID
					&& idtable.table[lextable.table[i].idxTI].idtype != IT::IDTYPE::F
					&& lextable.table[i + 1].lexeme == LEX_OPENING_PARENTHESIS)
				{
					throw ERROR_THROW_IN(709, lextable.table[i].lineNumber, 0);
				}
				else if (lextable.table[i].lexeme == LEX_CONCAT && datatype != IT::IDDATATYPE::STR
					|| lextable.table[i].lexeme == LEX_PARSE_INT && datatype != IT::IDDATATYPE::INT)
				{
					throw ERROR_THROW_IN(705, lextable.table[i].lineNumber, 0);
				}
			}
		}
	}
}

void SA::SemanticAnalyzer::FuncReturn()
{
	for (int i = 0; i < lextable.size; i++)
		if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F
			&& lextable.table[i - 1].lexeme == LEX_FN) {
			IT::IDDATATYPE iddatatype = idtable.table[lextable.table[i].idxTI].iddatatype;
			while (lextable.table[i].lexeme != LEX_RET)
				i++;
			if (idtable.table[lextable.table[i + 1].idxTI].iddatatype != iddatatype)
				throw ERROR_THROW_IN(707, lextable.table[i].lineNumber, 0);
		}
}

void SA::SemanticAnalyzer::CorrectAmountOfParams()
{
	std::string buf;
	buf.reserve(ID_MAXSIZE);
	int parms = 0, funcparms = 0;
	for (int i = 0; i < lextable.size; i++)
		if (lextable.table[i].lexeme == LEX_ASSIGN) {
			while (lextable.table[i].lexeme != LEX_SEMICOLON) {
				i++;
				if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F) {
					buf = idtable.table[lextable.table[i].idxTI].id;
					while (lextable.table[i].lexeme != LEX_CLOSING_PARENTHESIS) {
						i++;
						if (lextable.table[i].lexeme == LEX_ID || lextable.table[i].lexeme == LEX_LITERAL)
							parms++;
					}
					for (int j = 0; j < i; j++)
						if (lextable.table[j].lexeme == LEX_ID && idtable.table[lextable.table[j].idxTI].idtype == IT::IDTYPE::F
							&& buf == idtable.table[lextable.table[j].idxTI].id) {
							j++;
							for (j; lextable.table[j].lexeme != LEX_CLOSING_PARENTHESIS; j++)
								if (lextable.table[j].lexeme == LEX_ID)
									funcparms++;
							break;
						}
				}
			}
			if (funcparms != parms)
				throw ERROR_THROW_IN(701, lextable.table[i].lineNumber, 0);
		}
}

void SA::SemanticAnalyzer::Start(Log::Logger& log)
{
	CheckReturn();
	ParamsOfFunc();
	ParamsOfStdFunc();
	Types();
	FuncReturn();
	CorrectAmountOfParams();
	log << "-------------------------\n";
	log << "—емантический анализ выполнен без ошибок\n";
}

SA::SemanticAnalyzer::SemanticAnalyzer(LT::LexTable lextable, IT::IdTable idtable)
	: lextable(lextable), idtable(idtable)
{	}