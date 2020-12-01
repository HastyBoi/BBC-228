#include "pch.h"
#include "Error.h"
#include "FST.h"
#include "Greibach.h"
#include "In.h"
#include "IT.h"
#include "Lexer.h"
#include "Log.h"
#include "LT.h"
#include "MFST.h"
#include "Parm.h"
#include "PolishNotation.h"
#include "SemanticAnalyzer.h"
#include "CodeGeneration.h"

static int allocCount = 0;

int main(int argc, char** argv)
{
	std::setlocale(LC_ALL, "rus");

	try
	{
		Parm::Parameters commandLineArguments(argc, argv);
		Log::Logger log(commandLineArguments.logFilePath());

		log.openFile();
		log << "---- Протокол ------ Дата: ";
		log << log.getCurrentDateTime() << " ------------\n";

		auto parametersList = commandLineArguments.getAllParameters();
		log << "---- Параметры --------\n";
		for (const auto& p : parametersList) {
			log << p << '\n';
		}

		/*In::InputFileReader in = In::InputFileReader();
		in.read(commandLineArguments.inFilePath(), commandLineArguments.outFilePath());

		log << "---- Исходные данные ------" << '\n' <<
			"Количество символов: " << in.fileSize() << '\n' <<
			"Проигнорировано: " << in.ignoredCharsCount() << '\n' <<
			"Количество строк: " << in.linesCount() << '\n';

		LT::LexTable lextable = LT::Create(in.fileSize());
		IT::IdTable idtable = IT::Create(in.fileSize());*/

		In::IN in = In::getin(commandLineArguments.inFilePath(), commandLineArguments.outFilePath());
		LT::LexTable lextable = LT::Create(in.size);
		IT::IdTable idtable = IT::Create(in.size);

		LA::Scan(lextable, idtable, in, commandLineArguments.outFilePath(), log);

		MFST_TRACE_START
			MFST::Mfst mfst(lextable, GRB::getGreibach());
		mfst.start(std::cout);

		SA::SemanticAnalyzer sema(lextable, idtable);
		sema.Start(log);

		CG::Generator CodeBuilder = CG::Generator(lextable, idtable, commandLineArguments.outFilePath());
		CodeBuilder.Start(log);

		LT::Delete(lextable);
		IT::Delete(idtable);
		log.closeFile();
	}
	catch (Error::ERROR e)
	{
		std::cerr << e.message << '\n';
	}

#ifdef _DEBUG
	int hasMemoryLeaks = _CrtDumpMemoryLeaks();
#else
	system("pause");
#endif // _DEBUG
	return 0;
}