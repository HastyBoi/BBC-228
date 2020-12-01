#pragma once
#include "pch.h"
#include "LT.h"
#include "It.h"
#include "Log.h"

namespace CG
{
	struct Generator
	{
		LT::LexTable lextable;
		IT::IdTable idtable;
		std::ofstream out;
		void Head();
		void Constants();
		void Data();
		void Code();

		Generator(LT::LexTable lexT, IT::IdTable idT, const char* out);
		void Start(Log::Logger& log);
	};
}