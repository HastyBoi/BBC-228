#include "pch.h"
#include "Log.h"

constexpr auto timeBufferMaxSize = 228;

//void WriteIn(Log::LOG log, TTM::IN in)
//{
//	*log.stream << "---- Исходные данные ------" << '\n' <<
//		"Количество символов: " << in.size << '\n' <<
//		"Проигнорировано: " << in.ignor << '\n' <<
//		"Количество строк: " << in.lines << '\n';
//}

//void Log::WriteError(LOG log, Error::ERROR error)
//{
//	std::ostream& output = log.stream && log.stream->is_open() ? *log.stream : std::cout;
//
//	output << "Ошибка " << error.id << ": " << error.message;
//	error.inext.col + error.inext.line >= 0 ? output <<
//		", строка " << error.inext.line <<
//		", позиция " << error.inext.col << '\n' : output << '\n';
//}

Log::Logger::Logger(std::string_view logFilePath)
{
	setLogFilePath(logFilePath);
}

Log::Logger::~Logger()
{
	closeFile();
}

void Log::Logger::setLogFilePath(std::string_view logFilePath)
{
	if (logFilePath.empty())
		throw ERROR_THROW(112);

	if (m_logStream.is_open())
		m_logStream.close();

	m_logFilePath = logFilePath;
}

bool Log::Logger::reopenFile()
{
	closeFile();
	return openFile();
}

std::string Log::Logger::getCurrentDateTime() const
{
	char timeBuffer[timeBufferMaxSize];
	tm currentTime;
	time_t now = time(0);

	localtime_s(&currentTime, &now);
	strftime(timeBuffer, sizeof(timeBuffer), "%c", &currentTime);

	return timeBuffer;
}

bool Log::Logger::openFile()
{
	if (m_logFilePath.empty())
		return false;

	bool isOpen = m_logStream.is_open();
	if (!isOpen)
	{
		m_logStream.open(m_logFilePath);

		if (m_logStream.fail())
			throw ERROR_THROW(112);

		isOpen = true;
	}

	return isOpen;
}

void Log::Logger::closeFile()
{
	if (!m_logStream.is_open())
		return;

	m_logStream.close();
}