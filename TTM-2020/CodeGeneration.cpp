#include "pch.h"
#include "CodeGeneration.h"

TTM::Generator::Generator(LexTable& lextable, IdTable& idtable, const char* outFilePath)
	:lextable(lextable), idtable(idtable), outFile(std::ofstream(outFilePath))
{	}

void TTM::Generator::Start(Logger& log)
{
	Head();
	Constants();
	Data();
	Code();

	log << "Код сгенерирован\n";
}

void TTM::Generator::Head()
{
	outFile << ".586\n"
		<< ".model flat, stdcall\n"
		<< "includelib libucrt.lib\n"
		<< "includelib kernel32.lib\n"
		<< "ExitProcess PROTO : DWORD\n";
	if (lextable[0].lexeme == LEX_USE)
	{
		outFile << includeStdlib();
	}
	outFile << "\n.stack 4096\n";
}

void TTM::Generator::Constants()
{
	outFile << "\n.const\n"
		<< "_DIVIDE_BY_ZERO_EXCEPTION BYTE 'деление на 0',0\n";
	for (int i = 0; i < idtable.size(); ++i)
	{
		if (idtable[i].idType == it::id_type::literal)
		{
			outFile << getFullName(i);
			if (idtable[i].dataType == it::data_type::i32)
			{
				outFile << " DWORD " << idtable[i].value.intValue;
			}
			else if (idtable[i].dataType == it::data_type::str)
			{
				outFile << " BYTE " << idtable[i].value.strValue.string << ",0";
			}
			outFile << '\n';
		}
	}
}

void TTM::Generator::Data()
{
	outFile << "\n.data\n";
	for (int i = 0; i < idtable.size(); ++i)
	{
		if (idtable[i].idType == it::id_type::variable)
		{
			outFile << getFullName(i) << " DWORD ";
			if (idtable[i].dataType == it::data_type::i32)
			{
				outFile << idtable[i].value.intValue;
			}
			else if (idtable[i].dataType == it::data_type::str)
			{
				outFile << idtable[i].value.strValue.string;
			}
			outFile << '\n';
		}
	}
}

void TTM::Generator::Code()
{
	outFile << "\n.code\n";
	int assignToVariableAtIndex = 0;
	std::string lastFunctionName = "";

	for (int i = 0; i < lextable.size(); ++i)
	{
		switch (lextable[i].lexeme)
		{
		case LEX_ASSIGN:
			assignToVariableAtIndex = i - 1;
			while (lextable[i].lexeme != LEX_SEMICOLON)
			{
				if (lextable[i].lexeme == LEX_ID)
				{
					outFile << "push " << getFullName(lextable[i].idTableIndex) << '\n';
				}
				else if (lextable[i].lexeme == LEX_LITERAL)
				{
					outFile << "push ";
					if (idtable[lextable[i].idTableIndex].dataType == it::data_type::str)
					{
						outFile << "offset ";
					}

					outFile << getFullName(lextable[i].idTableIndex) << '\n';
				}
				else if (lextable[i].lexeme == LEX_FUNCTION_CALL)
				{
					/*outFile << "call " << getFullName(i) << '\n'
						<< "push eax";*/
				}
				else if (lextable[i].lexeme == LEX_PLUS)
				{
					outFile << "pop eax\n"
						<< "pop ebx\n"
						<< "add eax, ebx\n"
						<< "push eax\n";
				}
				else if (lextable[i].lexeme == LEX_MINUS)
				{
					outFile << "pop eax\n"
						<< "pop ebx\n"
						<< "sub eax, ebx\n"
						<< "push eax\n";
				}
				else if (lextable[i].lexeme == LEX_ASTERISK)
				{
					outFile << "pop eax\n"
						<< "pop ebx\n"
						<< "mul ebx\n"
						<< "push eax\n";
				}
				else if (lextable[i].lexeme == LEX_SLASH)
				{
					outFile << "pop ebx\n"
						<< "mov edx, 0\n"
						<< "pop eax\n"
						<< ".if ebx == 0\n"
						<< "push offset _DIVIDE_BY_ZERO_EXCEPTION\n"
						<< "call echoStr\n"
						<< "invoke ExitProcess, -1\n"
						<< ".endif\n"
						<< "div ebx\n"
						<< "push eax\n";
				}
				else if (lextable[i].lexeme == LEX_PERCENT)
				{
					outFile << "pop ebx\n"
						<< "mov edx, 0\n"
						<< "pop eax\n"
						<< ".if ebx == 0\n"
						<< "push offset _DIVIDE_BY_ZERO_EXCEPTION\n"
						<< "call echoStr\n"
						<< "invoke ExitProcess, -1\n"
						<< ".endif\n"
						<< "div ebx\n"
						<< "push edx\n";
				}

				++i;
			}

			outFile << "pop " << getFullName(lextable[assignToVariableAtIndex].idTableIndex) << '\n';

			break;

		case LEX_FN:
		case LEX_MAIN:
			lastFunctionName = getFullName(lextable[i + 2].idTableIndex);
			outFile << lastFunctionName << " PROC ";
			while (lextable[i].lexeme != LEX_CLOSING_PARENTHESIS)
			{
				if (lextable[i].lexeme == LEX_ID && idtable[lextable[i].idTableIndex].idType != it::id_type::function)
				{
					outFile << getFullName(lextable[i].idTableIndex) << " : DWORD ";
				}

				++i;
			}
			outFile << '\n';

			break;

		case LEX_ECHO:
			if (lextable[i + 1].lexeme == LEX_ID || lextable[i + 1].lexeme == LEX_LITERAL)
			{
				if (idtable[lextable[i + 1].idTableIndex].dataType == it::data_type::i32)
				{
					outFile << "push " << getFullName(lextable[i + 1].idTableIndex)
						<< "\ncall echoInt\n";
				}
				else if (idtable[lextable[i + 1].idTableIndex].dataType == it::data_type::str)
				{
					outFile << "push offset " << getFullName(lextable[i + 1].idTableIndex)
						<< "\ncall echoStr\n";
				}
			}
			break;

		case LEX_RET:
			outFile << "push " << getFullName(lextable[i + 1].idTableIndex) << '\n';

			if (lastFunctionName == "_main")
			{
				outFile << "call ExitProcess\n";
			}
			outFile << lastFunctionName << " ENDP\n\n";
			break;

		default:
			break;
		}
	}

	outFile << "end _main\n";
}

std::string TTM::Generator::getFullName(int index)
{
	return '_' + idtable[index].scope + idtable[index].name;
}

std::string TTM::Generator::includeStdlib()
{
	std::stringstream output;

	output << "includelib " << stdlibPath
		<< "\nechoInt PROTO : DWORD\n"
		<< "echoStr PROTO : DWORD\n"
		<< "parseInt PROTO : DWORD\n"
		<< "concat PROTO : DWORD\n";

	return output.str();
}