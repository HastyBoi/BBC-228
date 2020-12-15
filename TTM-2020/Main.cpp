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

		log << "---- �������� ------ ����: ";
		log << log.getCurrentDateTime() << " ------------\n";

		auto parametersList = commandLineArguments.getAllParameters();
		log << "---- ��������� --------\n";
		for (std::string p : parametersList)
			log << p << '\n';

		InputFileReader in;
		in.read(commandLineArguments.inFilePath());

		log << "---- �������� ������ ------" << '\n' <<
			"���������� ��������: " << in.fileSize() << '\n' <<
			"���������������: " << in.ignoredCharsCount() << '\n' <<
			"���������� �����: " << in.linesCount() << '\n'
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
			log << "������� ������ �������� � ����\n";
		}
		if (commandLineArguments.idTableFilePath())
		{
			std::ofstream idTableFile(commandLineArguments.idTableFilePath());
			idTableFile << idtable.dumpTable();
			idTableFile.close();
			log << "������� ��������������� �������� � ����\n";
		}
		if (commandLineArguments.traceFilePath())
		{
			std::ofstream traceFile(commandLineArguments.traceFilePath());
			traceFile << syntaxAnalyzer.dumpTrace();
			traceFile.close();
			log << "�������������� ������ ������� � ����\n";
		}
		if (commandLineArguments.rulesFilePath())
		{
			std::ofstream rulesFile(commandLineArguments.rulesFilePath());
			rulesFile << syntaxAnalyzer.getRules();
			rulesFile.close();
			log << "������� ������ �������� � ����\n";
		}
		log << "-----------------------------------------------------------\n";

		log.closeFile();
		std::cout << "������ ��������� �������. ���������� �������� � ���� " << commandLineArguments.logFilePath() << '\n';
	}
	catch (Error::ERROR e)
	{
		std::cerr << "������. ���������� ����������� � log-�����\n";
		log << "[������ " << e.id << "] ";
		log << e.message << ' ';
		if (e.inext.line > 0)
		{
			log << "������ " << e.inext.line << ' ';
		}
		if (e.inext.col > 0)
		{
			log << "������� " << e.inext.col << ' ';
		}
		log << '\n';
	}

	return 0;
}