#include "pch.h"
#include "Logger.h"

constexpr auto timeBufferMaxSize = 228;

TTM::Logger::Logger(std::string_view logFilePath)
{
	setLogFilePath(logFilePath);
}

TTM::Logger::~Logger()
{
	closeFile();
}

void TTM::Logger::setLogFilePath(std::string_view logFilePath)
{
	if (logFilePath.empty())
		throw ERROR_THROW(112);

	if (m_logStream.is_open())
		m_logStream.close();

	m_logFilePath = logFilePath;
}

bool TTM::Logger::reopenFile()
{
	closeFile();
	return openFile();
}

std::string TTM::Logger::getCurrentDateTime() const
{
	char timeBuffer[timeBufferMaxSize];
	tm currentTime;
	time_t now = time(0);

	localtime_s(&currentTime, &now);
	strftime(timeBuffer, sizeof(timeBuffer), "%c", &currentTime);

	return timeBuffer;
}

bool TTM::Logger::openFile()
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

void TTM::Logger::closeFile()
{
	if (!m_logStream.is_open())
		return;

	m_logStream.close();
}