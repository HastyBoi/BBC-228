#include "pch.h"
#include "Lexer.h"
#include "FST.h"

char LA::Tokenize(const char* string) {
	FST::FST nanomachinesSon[] = {
		FST_I32, FST_STR, FST_FN, FST_PARSE_INT, FST_IF, FST_ELSE, FST_CONCAT, FST_LET,
		FST_RETURN, FST_ECHO, FST_MAIN,
		FST_OPENING_PARENTHESIS, FST_CLOSING_PARENTHESIS, FST_SEMICOLON, FST_COMMA,
		FST_OPENING_CURLY_BRACE, FST_CLOSING_CURLY_BRACE,
		FST_PLUS, FST_MINUS, FST_ASTERISK, FST_SLASH, FST_PERCENT, FST_ASSIGN,
		FST_ID, FST_STRING_LITERAL, FST_INTEGER_LITERAL
	};
	const int size = sizeof(nanomachinesSon) / sizeof(nanomachinesSon[0]);
	const char tokens[] = {
		LEX_I32, LEX_STR, LEX_FN, LEX_PARSE_INT, LEX_IF, LEX_ELSE, LEX_CONCAT, LEX_LET,
		LEX_RET, LEX_ECHO, LEX_MAIN,
		LEX_OPENING_PARENTHESIS, LEX_CLOSING_PARENTHESIS, LEX_SEMICOLON, LEX_COMMA,
		LEX_OPENING_CURLY_BRACE, LEX_CLOSING_CURLY_BRACE,
		LEX_PLUS, LEX_MINUS, LEX_ASTERISK, LEX_SLASH, LEX_PERCENT, LEX_ASSIGN,
		LEX_ID, LEX_STRING_LITERAL, LEX_INTEGER_LITERAL
	};

	for (int i = 0; i < size; ++i) {
		if (execute(string, nanomachinesSon[i])) {
			return tokens[i];
		}
	}

	return EOF;
}

//void LA::Scan(LT::LexTable& lextable, IT::IdTable& idtable, In::IN& in, const char* outFilePath, Log::Logger& log) {
//	std::ofstream outfile(outFilePath);
//	outfile << "01 ";
//
//	std::string accumulator = "";
//	std::string curScope = "";
//	std::string prevScope = "";
//	std::string curFunction = "";
//
//	bool declaredNewFunction = false;
//
//	for (int i = 0, line = 1; in.code[i]; ++i) {
//		IT::IDDATATYPE iddatatype;
//		auto fillTables = [&] {
//			char token = Tokenize(accumulator.c_str());
//			if (token == EOF) {
//				log << accumulator << " - ";
//				throw ERROR_THROW(129);
//			}
//
//			int ti_idx = TI_NULLIDX;
//
//			iddatatype = (token == LEX_I32 || token == LEX_INTEGER_LITERAL) ? IT::IDDATATYPE::INT :
//				(token == LEX_STR || token == LEX_STRING_LITERAL) ? IT::IDDATATYPE::STR : iddatatype;
//
//			if (token == LEX_OPENING_PARENTHESIS) {
//				declaredNewFunction = lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_FN;
//
//				if (declaredNewFunction) {
//					prevScope = curScope;
//					curScope += curFunction;
//				}
//			}
//			else if (token == LEX_CLOSING_PARENTHESIS && declaredNewFunction) {
//				curScope = prevScope;
//				declaredNewFunction = false;
//			}
//			else if (token == LEX_OPENING_CURLY_BRACE) {
//				prevScope = curScope;
//				curScope += curFunction;
//			}
//			else if (token == LEX_CLOSING_CURLY_BRACE) {
//				curScope.clear();
//				curFunction.clear();
//			}
//			else if (token == LEX_INTEGER_LITERAL) {
//				int literal = std::stoi(accumulator);
//				ti_idx = IT::IsLiteral(idtable, literal);
//
//				if (ti_idx == TI_NULLIDX) {
//					IT::Add(idtable, { lextable.size, "", "", IT::IDTYPE::L, literal });
//				}
//
//				token = LEX_LITERAL;
//			}
//			else if (token == LEX_STRING_LITERAL) {
//				std::string literal = accumulator.substr(1, accumulator.size() - 2);
//				ti_idx = IT::IsLiteral(idtable, literal.c_str());
//
//				if (ti_idx == TI_NULLIDX) {
//					IT::Add(idtable, { lextable.size, "", "", IT::IDTYPE::L, literal.c_str() });
//				}
//
//				token = LEX_LITERAL;
//			}
//			else if (token == LEX_ID) {
//				std::string id = accumulator.substr(0, ID_MAXSIZE);
//				ti_idx = IT::IsId(idtable, curScope.c_str(), id.c_str());
//
//				if (ti_idx == TI_NULLIDX) {
//					if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_LET
//						&& lextable.table[lextable.size - 1].lexeme == LEX_DATATYPE) {
//						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::V });
//					}
//					else if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_FN) {
//						curFunction = id;
//						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::F });
//					}
//					else if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_DATATYPE
//						&& declaredNewFunction) {
//						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::P });
//					}
//					else {
//						log << accumulator << " - ";
//						throw ERROR_THROW(124);
//					}
//				}
//				else if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_LET
//					|| lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_FN) {
//					log << accumulator << " - ";
//					throw ERROR_THROW(123);
//				}
//			}
//			else if (token == LEX_MAIN && curScope.empty()) {
//				ti_idx = IT::IsId(idtable, "", accumulator.c_str());
//
//				if (ti_idx != TI_NULLIDX) {
//					log << accumulator << " - ";
//					throw ERROR_THROW(131);
//				}
//				curScope.clear();
//				curFunction = accumulator;
//
//				IT::Add(idtable, { lextable.size, "", accumulator.c_str(), IT::IDTYPE::F, 0 });
//			}
//
//			token = (token == LEX_I32 || token == LEX_STR) ? LEX_DATATYPE : token;
//
//			if (token == LEX_ID || token == LEX_LITERAL || token == LEX_MAIN) {
//				if (ti_idx == TI_NULLIDX) {
//					LT::Add(lextable, { token, line, idtable.size - 1 });
//				}
//				else {
//					LT::Add(lextable, { token, line, ti_idx });
//				}
//			}
//			else {
//				LT::Add(lextable, { token, line, LT_TI_NULLIDX });
//			}
//
//			outfile << token;
//			accumulator.clear();
//		};
//
//		char ch = in.text[i];
//		if (in.code[ch] == In::IN::T) {
//			if (ch != IN_CODE_ENDL)
//				accumulator += ch;
//		}
//		else if (in.code[ch] == In::IN::Q) {
//			do {
//				accumulator += ch;
//				i++;
//				ch = in.text[i];
//			} while (in.code[ch] != In::IN::Q);
//			accumulator += ch;
//			fillTables();
//		}
//		else if (in.code[ch] == In::IN::S || in.code[ch] == In::IN::O || ch == IN_CODE_DELIM) {
//			if (!accumulator.empty()) {
//				fillTables();
//			}
//			if (in.code[ch] == In::IN::O) {
//				accumulator = ch;
//				fillTables();
//			}
//		}
//
//		if (ch == IN_CODE_DELIM) {
//			line++;
//			if (in.text[i + 1]) {
//				outfile << '\n' << std::setw(2) << std::setfill('0') << line << " ";
//				i++;
//			}
//		}
//	}
//
//	if (IT::IsId(idtable, "", "main") == TI_NULLIDX) {
//		throw ERROR_THROW(130);
//	}
//
//	outfile.close();
//}

void LA::Scan(LT::LexTable& lextable, IT::IdTable& idtable, In::IN& in, const char* outFilePath, Log::Logger& log) {
	std::ofstream outfile(outFilePath);
	outfile << "01 ";

	std::string accumulator = "";
	std::string curScope = "";
	std::string prevScope = "";
	std::string curFunction = "";

	int bidlaSchetchik = 0;

	bool declaredNewFunction = false;

	for (int i = 0, line = 1; in.text[i]; ++i) {
		IT::IDDATATYPE iddatatype;
		auto fillTables = [&] {
			char token = Tokenize(accumulator.c_str());
			if (token == EOF) {
				log << accumulator << " - ";
				throw ERROR_THROW(129);
			}

			int ti_idx = TI_NULLIDX;

			iddatatype = (token == LEX_I32 || token == LEX_INTEGER_LITERAL) ? IT::IDDATATYPE::INT :
				(token == LEX_STR || token == LEX_STRING_LITERAL) ? IT::IDDATATYPE::STR : iddatatype;

			if (token == LEX_OPENING_PARENTHESIS) {
				declaredNewFunction = lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_FN;

				if (declaredNewFunction) {
					prevScope = curScope;
					curScope += curFunction;
				}
			}
			else if (token == LEX_CLOSING_PARENTHESIS && declaredNewFunction) {
				curScope = prevScope;
				declaredNewFunction = false;
			}
			else if (token == LEX_OPENING_CURLY_BRACE) {
				prevScope = curScope;
				curScope += curFunction;
			}
			else if (token == LEX_CLOSING_CURLY_BRACE) {
				curScope.clear();
				curFunction.clear();
			}
			else if (token == LEX_INTEGER_LITERAL) {
				int literal = std::stoi(accumulator);
				ti_idx = IT::IsLiteral(idtable, literal);

				if (ti_idx == TI_NULLIDX) {
					std::string name = "L";
					name += std::to_string(bidlaSchetchik);
					IT::Add(idtable, { lextable.size, "", name.c_str(), IT::IDTYPE::L, literal });
					++bidlaSchetchik;
				}

				token = LEX_LITERAL;
			}
			else if (token == LEX_STRING_LITERAL) {
				std::string literal = accumulator.substr(1, accumulator.size() - 2);
				ti_idx = IT::IsLiteral(idtable, literal.c_str());

				if (ti_idx == TI_NULLIDX) {
					std::string name = "L";
					name += std::to_string(bidlaSchetchik);
					IT::Add(idtable, { lextable.size, "", name.c_str(), IT::IDTYPE::L, literal.c_str() });
					++bidlaSchetchik;
				}

				token = LEX_LITERAL;
			}
			else if (token == LEX_ID) {
				std::string id = accumulator.substr(0, ID_MAXSIZE);
				ti_idx = IT::IsId(idtable, curScope.c_str(), id.c_str());

				if (ti_idx == TI_NULLIDX) {
					if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_LET
						&& lextable.table[lextable.size - 1].lexeme == LEX_DATATYPE) {
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::V });
					}
					else if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_FN) {
						curFunction = id;
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::F });
					}
					else if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_DATATYPE
						&& declaredNewFunction) {
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::P });
					}
					else {
						log << accumulator << " - ";
						throw ERROR_THROW(124);
					}
				}
				else if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_LET
					|| lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_FN) {
					log << accumulator << " - ";
					throw ERROR_THROW(123);
				}
			}
			else if (token == LEX_MAIN && curScope.empty()) {
				ti_idx = IT::IsId(idtable, "", accumulator.c_str());

				if (ti_idx != TI_NULLIDX) {
					log << accumulator << " - ";
					throw ERROR_THROW(131);
				}
				curScope.clear();
				curFunction = accumulator;

				IT::Add(idtable, { lextable.size, "", accumulator.c_str(), IT::IDTYPE::F, 0 });
			}

			token = (token == LEX_I32 || token == LEX_STR) ? LEX_DATATYPE : token;

			if (token == LEX_ID || token == LEX_LITERAL || token == LEX_MAIN) {
				if (ti_idx == TI_NULLIDX) {
					LT::Add(lextable, { token, line, idtable.size - 1 });
				}
				else {
					LT::Add(lextable, { token, line, ti_idx });
				}
			}
			else {
				LT::Add(lextable, { token, line, LT_TI_NULLIDX });
			}

			outfile << token;
			accumulator.clear();
		};

		unsigned char& ch = in.text[i];
		if (in.code[ch] == In::IN::T) {
			accumulator += ch;
		}
		else if (in.code[ch] == In::IN::Q) {
			do {
				accumulator += ch;
				i++;
				ch = in.text[i];
			} while (in.code[ch] != In::IN::Q);
			accumulator += ch;
			fillTables();
		}
		else if (in.code[ch] == In::IN::S || in.code[ch] == In::IN::O || ch == IN_CODE_DELIM) {
			if (!accumulator.empty()) {
				fillTables();
			}
			if (in.code[ch] == In::IN::O) {
				accumulator = ch;
				fillTables();
			}
		}

		if (ch == IN_CODE_DELIM) {
			line++;
			if (in.text[i + 1]) {
				outfile << "\n" << std::setw(2) << std::setfill('0') << line << " ";
				i++;
			}
		}
	}

	if (IT::IsId(idtable, "", "main") == TI_NULLIDX) {
		throw ERROR_THROW(130);
	}

	outfile.close();
}