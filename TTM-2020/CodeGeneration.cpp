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
		<< "ExitProcess PROTO : DWORD\n"
		<< includeStdlib() << '\n'
		<< ".stack 4096\n";
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
				outFile << " BYTE " << idtable[i].value.strValue.string << ", 0";
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
			outFile << getFullName(i) << " DWORD 0\n";
		}
	}
}

void TTM::Generator::Code()
{
	outFile << "\n.code\n";
	std::string lastFunctionName = "";
	int parametersCount = 0;

	for (int i = 0; i < lextable.size(); ++i)
	{
		switch (lextable[i].lexeme)
		{
		case LEX_ASSIGN:
			outFile << doOperations(i + 1);
			outFile << "pop " << getFullName(lextable[i - 1].idTableIndex) << '\n';
			break;

		case LEX_FN:
		case LEX_MAIN:
			lastFunctionName = getFullName(lextable[i + 2].idTableIndex);
			outFile << lastFunctionName << " PROC ";
			while (lextable[i].lexeme != LEX_CLOSING_PARENTHESIS)
			{
				if (lextable[i].lexeme == LEX_ID && idtable[lextable[i].idTableIndex].idType != it::id_type::function)
				{
					parametersCount += 4;
					outFile << getFullName(lextable[i].idTableIndex) << " : DWORD";
					if (lextable[i + 1].lexeme != LEX_CLOSING_PARENTHESIS)
						outFile << ", ";
				}

				++i;
			}
			outFile << '\n';
			break;

		case LEX_ECHO:
			writeEcho(i);
			break;

		case LEX_IF:
			i = writeIf(i);
			break;

		case LEX_RET:
			outFile << doOperations(i + 1);

			if (lastFunctionName == "_main")
			{
				outFile << "call ExitProcess\n";
			}
			else
			{
				outFile << "pop ";
				if (idtable[lextable[i + 1].idTableIndex].dataType == it::data_type::str)
				{
					outFile << "offset ";
				}

				outFile << "eax \n"
					<< "ret " << parametersCount << '\n';
				parametersCount = 0;
			}
			outFile << lastFunctionName << " ENDP\n\n";
			break;

		default:
			break;
		}
	}

	outFile << "end _main\n";
}

int TTM::Generator::writeIf(int startIndex)
{
	outFile << ".if " << getFullName(lextable[startIndex + 1].idTableIndex) << " != 0\n";

	int i = 0;
	for (i = startIndex; lextable[i].lexeme != LEX_CLOSING_CURLY_BRACE; ++i)
	{
		if (lextable[i].lexeme == LEX_ASSIGN) {
			outFile << doOperations(i + 1);
			outFile << "pop " << getFullName(lextable[i - 1].idTableIndex) << '\n';
		}
		else if (lextable[i].lexeme == LEX_ECHO)
		{
			writeEcho(i);
		}
	}
	if (lextable[i + 1].lexeme == LEX_ELSE)
	{
		outFile << ".else\n";
		for (i = i + 1; lextable[i].lexeme != LEX_CLOSING_CURLY_BRACE; ++i)
		{
			if (lextable[i].lexeme == LEX_ASSIGN) {
				outFile << doOperations(i + 1);
				outFile << "pop " << getFullName(lextable[i - 1].idTableIndex) << '\n';
			}
			else if (lextable[i].lexeme == LEX_ECHO)
			{
				writeEcho(i);
			}
		}
	}

	outFile << ".endif\n";
	return i;
}

void TTM::Generator::writeEcho(int startIndex)
{
	for (int i = startIndex; lextable[i].lexeme != LEX_SEMICOLON; ++i)
	{
		if (lextable[i + 1].lexeme == LEX_ID || lextable[i + 1].lexeme == LEX_LITERAL)
		{
			if (idtable[lextable[i + 1].idTableIndex].dataType == it::data_type::i32)
			{
				outFile << "push " << getFullName(lextable[i + 1].idTableIndex)
					<< "\ncall _echoInt\n";
			}
			else if (idtable[lextable[i + 1].idTableIndex].dataType == it::data_type::str)
			{
				outFile << "push ";
				if (idtable[lextable[i + 1].idTableIndex].idType == it::id_type::literal)
				{
					outFile << "offset ";
				}
				outFile << getFullName(lextable[i + 1].idTableIndex)
					<< "\ncall _echoStr\n";
			}
		}
	}
}

std::string TTM::Generator::getFullName(int index)
{
	return '_' + idtable[index].scope + idtable[index].name;
}

std::string TTM::Generator::includeStdlib()
{
	std::stringstream output;

	output << "includelib " << stdlibPath
		<< "\n_echoInt PROTO : DWORD\n"
		<< "_echoStr PROTO : DWORD\n"
		<< "_parseInt PROTO : DWORD\n"
		<< "_concat PROTO : DWORD, : DWORD\n";

	return output.str();
}

std::string TTM::Generator::doOperations(int startIndex)
{
	std::stringstream output;
	for (int i = startIndex; i < lextable.size() && lextable[i].lexeme != LEX_SEMICOLON; ++i)
	{
		if (lextable[i].lexeme == LEX_ID)
		{
			output << "push " << getFullName(lextable[i].idTableIndex) << '\n';
		}
		else if (lextable[i].lexeme == LEX_LITERAL)
		{
			output << "push ";
			if (idtable[lextable[i].idTableIndex].dataType == it::data_type::str)
			{
				output << "offset ";
			}

			output << getFullName(lextable[i].idTableIndex) << '\n';
		}
		else if (lextable[i].lexeme == LEX_FUNCTION_CALL)
		{
			output << "call " << getFullName(lextable[i].idTableIndex) << '\n';
			output << "push eax\n";
		}
		else if (lextable[i].lexeme == LEX_PLUS)
		{
			output << "pop eax\n"
				<< "pop ebx\n"
				<< "add eax, ebx\n"
				<< "push eax\n";
		}
		else if (lextable[i].lexeme == LEX_MINUS)
		{
			output << "pop ebx\n"
				<< "pop eax\n"
				<< "sub eax, ebx\n"
				<< "push eax\n";
		}
		else if (lextable[i].lexeme == LEX_ASTERISK)
		{
			output << "pop eax\n"
				<< "pop ebx\n"
				<< "mul ebx\n"
				<< "push eax\n";
		}
		else if (lextable[i].lexeme == LEX_SLASH)
		{
			output << "pop ebx\n"
				<< "mov edx, 0\n"
				<< "pop eax\n"
				<< ".if ebx == 0\n"
				<< "push offset _DIVIDE_BY_ZERO_EXCEPTION\n"
				<< "call _echoStr\n"
				<< "invoke ExitProcess, -1\n"
				<< ".endif\n"
				<< "div ebx\n"
				<< "push eax\n";
		}
		else if (lextable[i].lexeme == LEX_PERCENT)
		{
			output << "pop ebx\n"
				<< "mov edx, 0\n"
				<< "pop eax\n"
				<< ".if ebx == 0\n"
				<< "push offset _DIVIDE_BY_ZERO_EXCEPTION\n"
				<< "call _echoStr\n"
				<< "invoke ExitProcess, -1\n"
				<< ".endif\n"
				<< "div ebx\n"
				<< "push edx\n";
		}
	}
	return output.str();
}