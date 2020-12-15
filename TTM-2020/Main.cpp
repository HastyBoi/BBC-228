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

int main(int argc, char** argv)
{
	using namespace TTM;
	std::setlocale(LC_ALL, "rus");
	Logger log{ };

	try
	{
		CommandLineArgumentsParser commandLineArguments{ argc, argv };
		log.setLogFilePath(commandLineArguments.logFilePath());
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

		PolishNotation(lextable, idtable);

		Generator codeGenerator{ lextable, idtable, commandLineArguments.outFilePath() };
		codeGenerator.Start(log);

		log << "-----------------------------------------------------------\n";
		if (commandLineArguments.lexTableFilePath())
		{
			std::ofstream lexTableFile(commandLineArguments.lexTableFilePath());
			lexTableFile << lextable.dumpTable();
			lexTableFile.close();
			log << "Таблица лексем записана в файл\n";
		}
		if (commandLineArguments.idTableFilePath())
		{
			std::ofstream idTableFile(commandLineArguments.idTableFilePath());
			idTableFile << idtable.dumpTable();
			idTableFile.close();
			log << "Таблица идентификаторов записана в файл\n";
		}
		if (commandLineArguments.traceFilePath())
		{
			std::ofstream traceFile(commandLineArguments.traceFilePath());
			traceFile << syntaxAnalyzer.dumpTrace();
			traceFile.close();
			log << "Синтаксический разбор записан в файл\n";
		}
		if (commandLineArguments.rulesFilePath())
		{
			std::ofstream rulesFile(commandLineArguments.rulesFilePath());
			rulesFile << syntaxAnalyzer.getRules();
			rulesFile.close();
			log << "Цепочки правил записаны в файл\n";
		}
		log << "-----------------------------------------------------------\n";

		log.closeFile();
		std::cout << "Работа завершена успешно. Информация записана в файл " << commandLineArguments.logFilePath() << '\n';
	}
	catch (Error::ERROR e)
	{
		std::cerr << "Ошибка. Информация расположена в log-файле\n";
		log << "[ошибка " << e.id << "] ";
		log << e.message << ' ';
		if (e.inext.line > 0)
		{
			log << "строка " << e.inext.line << ' ';
		}
		if (e.inext.col > 0)
		{
			log << "позиция " << e.inext.col << ' ';
		}
		log << '\n';
	}

	return 0;
}