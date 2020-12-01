#pragma once
#include "In.h"
#include "Parm.h"
#include "Error.h"

namespace Log {
	class Logger {
	public:
		Logger(std::string_view logFilePath = "log.txt");
		~Logger();

		void setLogFilePath(std::string_view logFilePath);
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