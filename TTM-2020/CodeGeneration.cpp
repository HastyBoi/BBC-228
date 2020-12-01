#include "pch.h"
#include "CodeGeneration.h"

void CG::Generator::Head()
{
	out << ".586\n";
	out << ".model flat, stdcall\n";
	out << "includelib libucrt.lib\n";
	out << "includelib kernel32.lib\n";
	out << "includelib lib/LP_Lib.lib\n";
	out << "ExitProcess PROTO : DWORD\n";
	//out << "EXTRN Concat\t\t: PROC\n";
	out << "ConvertToChar\t\t PROTO :DWORD\n";
	//out << "EXTRN Copy\t\t: PROC\n";
	out << "echoOut\t\t PROTO :DWORD\n\n";
	out << "\n.stack 4096\n";
}

void CG::Generator::Constants()
{
	out << ".const\n";
	for (int i = 0; i < idtable.size; i++)
		if (idtable.table[i].idtype == IT::IDTYPE::L)
		{
			out << "\t" << idtable.table[i].id;
			if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR)
				out << " BYTE '" << idtable.table[i].value.vstr.str << "', 0";
			if (idtable.table[i].iddatatype == IT::IDDATATYPE::INT)
				out << " DWORD " << std::setw(8) << std::setfill('0') << idtable.table[i].value.vint;
			out << '\n';
		}
}

void CG::Generator::Data()
{
	std::vector<char> operators = { LEX_MINUS, LEX_PLUS, LEX_SLASH, LEX_ASTERISK, LEX_PERCENT };
	out << ".data\n";
	for (int i = 0; i < idtable.size; i++)
		if (idtable.table[i].idtype == IT::IDTYPE::V) {
			out << '\t';
			out << idtable.table[i].scope << idtable.table[i].id;
			out << "\t\t\tDWORD 0\n";
		}
}

void CG::Generator::Code()
{
	out << "\n.code\n";
	int indOfFunc = -1;
	int indOflex = -1;
	bool func = false;
	bool main = false;
	for (int i = 0; i < lextable.size; i++) {
		switch (lextable.table[i].lexeme) {
		case LEX_FN: {
			if (func || main)
				break;
			indOfFunc = i + 1;
			out << idtable.table[lextable.table[indOfFunc].idxTI].id << " PROC ";
			func = true;
			int backup = i;
			while (lextable.table[i].lexeme != LEX_CLOSING_PARENTHESIS)
				i++;
			while (lextable.table[i].lexeme != LEX_OPENING_PARENTHESIS)
			{
				if (lextable.table[i].lexeme == LEX_ID)
				{
					out << idtable.table[lextable.table[i].idxTI].id << ": DWORD";
					if (lextable.table[i - 2].lexeme != LEX_OPENING_PARENTHESIS)
						out << ", ";
				}
				i--;
			}
			i = backup;
			out << std::endl;
			break;
		}
		case LEX_MAIN: {
			main = true;
			out << "main PROC\n";
			break;
		}
		case LEX_CLOSING_CURLY_BRACE: {
			if (func)
			{
				out << idtable.table[lextable.table[indOfFunc].idxTI].id << " ENDP\n\n";
				func = false;
			}
			else
				out << "\tcall\t\tExitProcess\nmain ENDP\n";
			indOfFunc = 0;
			break;
		}
		case LEX_RET: {
			if (main) {
				out << "\tpush\t\t";
				if (lextable.table[i + 1].lexeme == LEX_ID) {
					out << idtable.table[lextable.table[i + 1].idxTI].scope
						<< idtable.table[lextable.table[i + 1].idxTI].id;
				}
				else
					out << idtable.table[lextable.table[i + 1].idxTI].id;
				out << std::endl;
			}
			else
			{
				if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::STR) {
					if (idtable.table[lextable.table[i + 1].idxTI].idtype != IT::IDTYPE::L)
						out << "\tmov\t\teax, " << idtable.table[lextable.table[i + 1].idxTI].scope
						<< idtable.table[lextable.table[i + 1].idxTI].id << "\n\tret\n";
					else
						out << "\tmov\t\teax, offset '" << idtable.table[lextable.table[i + 1].idxTI].id << "'\n\tret\n";
				}
				else
					out << "\tmov\t\teax, " << idtable.table[lextable.table[i + 1].idxTI].scope
					<< idtable.table[lextable.table[i + 1].idxTI].id << "\n\tret\n";
			}
			break;
		}
		case LEX_ECHO: {
			if (lextable.table[i + 1].lexeme == LEX_ID) {
				if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::INT) {
					out << "\tpush\t\t" << idtable.table[lextable.table[i + 1].idxTI].scope
						<< idtable.table[lextable.table[i + 1].idxTI].id;
					out << "\n\tcall\t\tConvertToChar\n";
					out << "\tpush\t\teax\n";
					out << "\tcall\t\techoOut\n\n";
				}
				else {
					out << "\tpush\t\t" << idtable.table[lextable.table[i + 1].idxTI].scope
						<< idtable.table[lextable.table[i + 1].idxTI].id;
					out << "\n\tcall\t\techoOut\n\n";
				}
			}
			else if (lextable.table[i + 1].lexeme == LEX_LITERAL) {
				if (idtable.table[lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::INT) {
					out << "\tpush\t\t" << idtable.table[lextable.table[i + 1].idxTI].id;
					out << "\n\tcall\t\tConvertToChar\n";
					out << "\tpush\t\teax\n";
					out << "\tcall\t\techoOut\n\n";
				}
				else {
					out << "\tpush\t\toffset " << idtable.table[lextable.table[i + 1].idxTI].id;
					out << "\n\tcall\t\techoOut\n\n";
				}
			}
			break;
		}
		case LEX_ASSIGN: {
			indOflex = i - 1;
			while (lextable.table[i].lexeme != LEX_SEMICOLON) {
				if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype != IT::IDTYPE::F)
					if (!func)
						out << "\tpush\t\t" << idtable.table[lextable.table[i].idxTI].scope
						<< idtable.table[lextable.table[i].idxTI].id << "\n";
					else
						out << "\tpush\t\t" << idtable.table[lextable.table[i].idxTI].id << "\n";
				if (lextable.table[i].lexeme == LEX_LITERAL) {
					if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT)
						out << "\tpush\t\t";
					else
						out << "\tpush\t\toffset ";
					out << idtable.table[lextable.table[i].idxTI].id << "\n";
				}
				if (lextable.table[i].lexeme == '@')
				{
					int delta = lextable.table[i + 1].lexeme - '0' + 1;
					out << "\tcall\t\t" << idtable.table[lextable.table[i - delta].idxTI].id << "\n";
					out << "\tpush\t\teax\n";
				}
				if (lextable.table[i].lexeme == LEX_PLUS)
				{
					out << "\t;\\/Сложение\\/\n";
					out << "\tpop\t\teax\n";
					out << "\tpop\t\tebx\n";
					out << "\tadd\t\teax, ebx\n";
					out << "\tpush\t\teax\n";
					out << "\t;/\\Сложение/\\\n";
				}
				if (lextable.table[i].lexeme == LEX_MINUS)
				{
					out << "\t;\\/Вычитание\\/\n";
					out << "\tpop\t\tebx\n";
					out << "\tpop\t\teax\n";
					out << "\tsub\t\teax, ebx\n";
					out << "\tpush\t\teax\n";
					out << "\t;/\\Вычитание/\\\n";
				}
				if (lextable.table[i].lexeme == LEX_ASTERISK)
				{
					out << "\t;\\/Умножение\\/\n";
					out << "\tpop\t\teax\n";
					out << "\tpop\t\tebx\n";
					out << "\tmul\t\tebx\n";
					out << "\tpush\t\teax\n";
					out << "\t;/\\Умножение/\\\n";
				}
				if (lextable.table[i].lexeme == LEX_SLASH) {
					out << "\t;\\/Деление\\/\n";
					out << "\tpop\t\tebx\n";
					out << "\tmov\t\tedx, 0\n";
					out << "\tpop\t\teax\n";
					out << "\tdiv\t\tebx\n";
					out << "\tpush\t\teax\n";
					out << "\t;/\\Деление/\\\n";
				}
				if (lextable.table[i].lexeme == LEX_PERCENT) {
					out << "\t;\\/Остаток от деления\\/\n";
					out << "\tpop\t\tebx\n";
					out << "\tmov\t\tedx, 0\n";
					out << "\tpop\t\teax\n";
					out << "\tdiv\t\tebx\n";
					out << "\tpush\t\tedx\n";
					out << "\t;/\\Остаток от деления/\\\n";
				}
				i++;
			}
			out << "\tpop\t\t\t" << idtable.table[lextable.table[indOflex].idxTI].scope
				<< idtable.table[lextable.table[indOflex].idxTI].id << "\n\n";
			break;
		}
		default:break;
		}
	}
	out << "end main\n";
}

CG::Generator::Generator(LT::LexTable lextable, IT::IdTable idtable, const char* outFilePath)
	:lextable(lextable), idtable(idtable), out(std::ofstream(outFilePath))
{	}

void CG::Generator::Start(Log::Logger& log)
{
	Head();
	Constants();
	Data();
	Code();
	log << "---\n";
	log << "Код успешно сгенерирован\n";
}