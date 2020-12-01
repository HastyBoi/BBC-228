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

void* operator new(size_t sz)
{
	allocCount++;
	return malloc(sz);
}

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
		for (std::string_view p : parametersList) {
			log << p << '\n';
		}

		TTM::InputFileReader in;
		in.read(commandLineArguments.inFilePath());

		auto hihi = TTM::InputFileReader::splitStringByDelimiter(in.fileText(), TTM::IN_CODE_DELIM);

		for (auto s : hihi) {
			std::cout << s << '\n';
		}

		LT::LexTable lextable = LT::Create(1488);
		IT::IdTable idtable = IT::Create(1488);
		/*LA::Scan(lextable, idtable, in, commandLineArguments.outFilePath(), log);

		MFST_TRACE_START
			MFST::Mfst mfst(lextable, GRB::getGreibach());
		mfst.start(std::cout);

		SA::SemanticAnalyzer sema(lextable, idtable);
		sema.Start(log);

		CG::Generator CodeBuilder = CG::Generator(lextable, idtable, commandLineArguments.outFilePath());
		CodeBuilder.Start(log);*/

		LT::Delete(lextable);
		IT::Delete(idtable);
		log.closeFile();
	}
	catch (Error::ERROR e)
	{
		std::cerr << e.message << '\n';
		std::cerr << "строка " << e.inext.line << " позиция " << e.inext.col << '\n';
	}

#ifdef _DEBUG
	int hasMemoryLeaks = _CrtDumpMemoryLeaks();
#else
	system("pause");
#endif // _DEBUG
	return 0;
}