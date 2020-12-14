#pragma once
#include "pch.h"
#include "LexTable.h"
#include "Logger.h"
#include "IdTable.h"

namespace TTM
{
	class Generator
	{
	public:
		Generator(LexTable& lextable, IdTable& idtable, const char* outFilePath);
		void Start(Logger& log);

	private:
		LexTable& lextable;
		IdTable& idtable;
		std::ofstream outFile;

		const char* stdlibPath = "../Debug/stdlib.lib";
		void Head();
		void Constants();
		void Data();
		void Code();

		std::string getFullName(int index);
		std::string includeStdlib();
		std::string doOperations(int startIndex);

		int writeIf(int startIndex);
		void writeEcho(int startIndex);
		int getFunctionCallIndex(int startIndex);
		bool isArgumentOfFunction(int index);
	};
}