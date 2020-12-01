#pragma once
#include <tchar.h>

#define PARM_IN L"-in:"
#define PARM_OUT L"-out:"
#define PARM_LOG L"-log:"
#define PARM_MAX_SIZE 300
#define PARM_OUT_DEFAULT_EXT L".out"
#define PARM_LOG_DEFAULT_EXT L".log"

namespace Parm
{
	class Parameters
	{
	public:
		Parameters(int argc, char** argv);

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

		static bool optionExists(char** begin, char** end, std::string_view option);
		static char* getOption(char** begin, char** end, std::string_view option);
	};
}