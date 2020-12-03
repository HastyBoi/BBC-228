#include "pch.h"
#include "SyntaxAnalyzer.h"
#include "Error.h"
#include "FST.h"
#include "Greibach.h"
#include "InputFileReader.h"
#include "IdTable.h"
#include "LexicalAnalyzer.h"
#include "Logger.h"
#include "LexTable.h"
#include "CommandLineArgumentsParser.h"
#include "PolishNotation.h"
#include "SemanticAnalyzer.h"
#include "CodeGeneration.h"
#include "LexicalAnalyzer.h"

//static int allocCount = 0;

//void* operator new(size_t sz)
//{
//	allocCount++;
//	return malloc(sz);
//}

int main(int argc, char** argv)
{
	using namespace TTM;
	std::setlocale(LC_ALL, "rus");

	try
	{
		CommandLineArgumentsParser commandLineArguments{ argc, argv };
		Logger log{ commandLineArguments.logFilePath() };

		log.openFile();
		log << "---- Протокол ------ Дата: ";
		log << log.getCurrentDateTime() << " ------------\n";

		auto parametersList = commandLineArguments.getAllParameters();
		log << "---- Параметры --------\n";
		for (std::string p : parametersList)
			log << p << '\n';

		InputFileReader in;
		in.read(commandLineArguments.inFilePath());

		log << "---- Исходные данные ------" << '\n' <<
			"Количество символов: " << in.fileSize() << '\n' <<
			"Проигнорировано: " << in.ignoredCharsCount() << '\n' <<
			"Количество строк: " << in.linesCount() << '\n'
			<< "-----------------------------------------------------------\n";

		auto splitted = InputFileReader::splitStringByDelimiter(in.fileText(), in::delimiter);
		LexTable lextable{ splitted.size() };
		IdTable idtable{};
		LexicalAnalyzer lexicalAnalyzer{ lextable, idtable };
		lexicalAnalyzer.Scan(splitted, log);
		SyntaxAnalyzer syntaxAnalyzer{ lextable, GRB::getGreibach() };
		syntaxAnalyzer.Start(log);

		SemanticAnalyzer semanticAnalyzer{ lextable, idtable };
		semanticAnalyzer.Start(log);

		if (commandLineArguments.lexTableFilePath())
		{
			std::ofstream lexTableFile(commandLineArguments.lexTableFilePath());
			lexTableFile << lextable.dumpTable();
			lexTableFile.close();
		}
		if (commandLineArguments.idTableFilePath())
		{
			std::ofstream idTableFile(commandLineArguments.idTableFilePath());
			idTableFile << idtable.dumpTable();
			idTableFile.close();
		}

		if (commandLineArguments.traceFilePath())
		{
			std::ofstream traceFile(commandLineArguments.traceFilePath());
			traceFile << syntaxAnalyzer.dumpTrace();
			traceFile.close();
		}

		log.closeFile();
	}
	catch (Error::ERROR e)
	{
		std::cerr << e.message << ' ';
		if (e.inext.line > 0)
		{
			std::cerr << "строка " << e.inext.line << ' ';
		}
		if (e.inext.col > 0)
		{
			std::cerr << "позиция " << e.inext.col << ' ';
		}
		std::cerr << '\n';
	}

#ifdef _DEBUG
	int hasMemoryLeaks = _CrtDumpMemoryLeaks();
#else
	system("pause");
#endif // _DEBUG
	return 0;
}