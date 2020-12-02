#pragma once
#include "InputFileReader.h"
#include "CommandLineArgumentsParser.h"
#include "Error.h"

namespace TTM
{
	class Logger
	{
	public:
		Logger(std::string logFilePath = "log.txt");
		~Logger();

		void setLogFilePath(std::string logFilePath);
		bool openFile();
		bool reopenFile();
		void closeFile();

		std::string getCurrentDateTime() const;

		template<class T>
		Logger& operator <<(T val);

	private:
		std::string m_logFilePath;
		std::ofstream m_logStream;
	};

	template<class T>
	inline Logger& Logger::operator<<(T val)
	{
		m_logStream << val;
		return *this;
	}
}