#pragma once
#include "LT.h"
#include "IT.h"
#include "Log.h"

namespace SA
{
	struct SemanticAnalyzer
	{
		LT::LexTable lextable;
		IT::IdTable idtable;
		void CheckReturn();
		void ParamsOfFunc();
		void ParamsOfStdFunc();
		void Types();
		void FuncReturn();
		void CorrectAmountOfParams();

		void Start(Log::Logger& log);
		SemanticAnalyzer(LT::LexTable lextable, IT::IdTable idtable);
	};
}