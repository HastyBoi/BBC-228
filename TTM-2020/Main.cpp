#include "pch.h"
#include "Error.h"
#include "FST.h"
#include "Greibach.h"
#include "InputFileReader.h"
#include "IdTable.h"
#include "LexicalAnalyzer.h"
#include "Logger.h"
#include "LexTable.h"
#include "MFST.h"
#include "CommandLineArgumentsParser.h"
#include "PolishNotation.h"
#include "SemanticAnalyzer.h"
#include "CodeGeneration.h"
#include "LexicalAnalyzer.h"

static int allocCount = 0;

//void* operator new(size_t sz)
//{
//	allocCount++;
//	return malloc(sz);
//}

int main(int argc, char** argv)
{
	using namespace TTM;
	std::srand((size_t)std::time(nullptr));
	std::setlocale(LC_ALL, "rus");

	try
	{
		CommandLineArgumentsParser commandLineArguments{ argc, argv };
		Logger log{ commandLineArguments.logFilePath() };

		log.openFile();
		log << "---- �������� ------ ����: ";
		log << log.getCurrentDateTime() << " ------------\n";

		auto parametersList = commandLineArguments.getAllParameters();
		log << "---- ��������� --------\n";
		for (std::string p : parametersList)
		{
			log << p << '\n';
		}

		InputFileReader in;
		in.read(commandLineArguments.inFilePath());

		log << "---- �������� ������ ------" << '\n' <<
			"���������� ��������: " << in.fileSize() << '\n' <<
			"���������������: " << in.ignoredCharsCount() << '\n' <<
			"���������� �����: " << in.linesCount() << '\n';

		auto splitted = InputFileReader::splitStringByDelimiter(in.fileText(), in::delimiter);
		LexTable lextable{ splitted.size() };
		IdTable idtable{};
		LexicalAnalyzer lexer{ lextable,idtable };
		lexer.Scan(splitted, log);

		log.closeFile();
	}
	catch (Error::ERROR e)
	{
		std::cerr << e.message << '\n';
		if (e.inext.line > 0 && e.inext.col > 0)
		{
			std::cerr << "������ " << e.inext.line << " ������� " << e.inext.col << '\n';
		}
	}

#ifdef _DEBUG
	int hasMemoryLeaks = _CrtDumpMemoryLeaks();
#else
	system("pause");
#endif // _DEBUG
	return 0;
}