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
		const char* lexTableFilePath() const { return m_lexTablePath.c_str(); }
		const char* idTableFilePath() const { return m_idTablePath.c_str(); }
		const char* traceFilePath() const { return m_tracePath.c_str(); }
		const char* rulesFilePath() const { return m_rulesPath.c_str(); }

		std::vector<std::string> getAllParameters() const;

	private:
		const char delimiter = '-';
		const std::string inKey = "in";
		const std::string outKey = "out";
		const std::string logKey = "log";
		const std::string lexKey = "lex";
		const std::string idKey = "id";
		const std::string traceKey = "trace";
		const std::string rulesKey = "rules";

		std::string m_inFilePath;
		std::string m_outFilePath;
		std::string m_logFilePath;
		std::string m_lexTablePath;
		std::string m_idTablePath;
		std::string m_tracePath;
		std::string m_rulesPath;

		static bool optionExists(char** begin, char** end, std::string option);
		static char* getOption(char** begin, char** end, std::string option);
	};
}