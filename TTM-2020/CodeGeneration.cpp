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
		<< "ExitProcess PROTO : SDWORD\n"
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
				outFile << " SDWORD " << idtable[i].value.intValue;
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
			outFile << getFullName(i) << " SDWORD 0\n";
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
					outFile << getFullName(lextable[i].idTableIndex) << " : SDWORD";
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
				if (lextable[i + 1].lexeme == LEX_LITERAL && idtable[lextable[i + 1].idTableIndex].dataType == it::data_type::str)
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
				if (lextable[i + 1].lexeme == LEX_LITERAL)
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
		<< "\n_echoInt PROTO : SDWORD\n"
		<< "_echoStr PROTO : SDWORD\n"
		<< "_parseInt PROTO : SDWORD\n"
		<< "_concat PROTO : SDWORD, : SDWORD\n";

	return output.str();
}

int TTM::Generator::getFunctionCallIndex(int startIndex)
{
	for (int i = startIndex; lextable[i].lexeme != LEX_SEMICOLON && lextable[i].lexeme != LEX_PLUS
		&& lextable[i].lexeme != LEX_MINUS && lextable[i].lexeme != LEX_ASTERISK
		&& lextable[i].lexeme != LEX_SLASH && lextable[i].lexeme != LEX_PERCENT; ++i)
	{
		if (lextable[i].lexeme == LEX_FUNCTION_CALL)
		{
			return i;
		}
	}

	return TI_NULLIDX;
}

bool TTM::Generator::isArgumentOfFunction(int index)
{
	int functionCallIndex = getFunctionCallIndex(index);
	if (functionCallIndex != TI_NULLIDX)
	{
		int parametersCount = lextable[functionCallIndex + 1].lexeme - '0';
		return (functionCallIndex - parametersCount) == index;
	}

	return false;
}

std::string TTM::Generator::doOperations(int startIndex)
{
	std::stringstream output;

	for (int i = startIndex; lextable[i].lexeme != LEX_SEMICOLON; ++i)
	{
		if (lextable[i].lexeme == LEX_FUNCTION_CALL)
		{
			output << "invoke " << getFullName(lextable[i].idTableIndex);
			int argumentsCount = lextable[i + 1].lexeme - '0';

			for (int j = argumentsCount; j > 0; --j)
			{
				output << ", ";
				if (lextable[i - j].lexeme == LEX_LITERAL && idtable[lextable[i - j].idTableIndex].dataType == it::data_type::str)
				{
					output << "offset ";
				}
				output << getFullName(lextable[i - j].idTableIndex);
			}
			output << "\npush eax\n";
		}
		else if (lextable[i].lexeme == LEX_ID && !isArgumentOfFunction(i))
		{
			output << "push " << getFullName(lextable[i].idTableIndex) << '\n';
		}
		else if (lextable[i].lexeme == LEX_LITERAL && !isArgumentOfFunction(i))
		{
			output << "push ";
			if (idtable[lextable[i].idTableIndex].dataType == it::data_type::str)
			{
				output << "offset ";
			}

			output << getFullName(lextable[i].idTableIndex) << '\n';
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
				<< "idiv ebx\n"
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
				<< "idiv ebx\n"
				<< "push edx\n";
		}
	}
	return output.str();
}