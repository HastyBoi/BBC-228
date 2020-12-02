#pragma once
#include <tchar.h>

namespace TTM
{
	class CommandLineArgumentsParser
	{
	public:
		CommandLineArgumentsParser(int argc, char** argv);

		const char* inFilePath() const { return m_inFilePath.c_str(); }
		const char* outFilePath() const { return m_outFilePath.c_str(); }
		const char* logFilePath() const { return m_logFilePath.c_str(); }

		std::vector<std::string> getAllParameters() const;

	private:
		const char delimiter = '-';
		const std::string defaultInKey = "in";
		const std::string defaultOutKey = "out";
		const std::string defaultLogKey = "log";

		std::string m_inFilePath;
		std::string m_outFilePath;
		std::string m_logFilePath;

		static bool optionExists(char** begin, char** end, std::string option);
		static char* getOption(char** begin, char** end, std::string option);
	};
}