#include "pch.h"
#include "CommandLineArgumentsParser.h"
#include "Error.h"

TTM::CommandLineArgumentsParser::CommandLineArgumentsParser(int argc, char** argv)
{
	if (argc <= 1 || getOption(argv + 1, argv + argc, delimiter + defaultInKey) == nullptr)
		throw ERROR_THROW(100);

	auto findPathByKey = [&](const std::string& key)
	{
		char* path = getOption(argv + 1, argv + argc, delimiter + key);
		return (path == nullptr) ? m_inFilePath + '.' + key : path;
	};

	m_inFilePath = findPathByKey(defaultInKey);
	m_outFilePath = findPathByKey(defaultOutKey);
	m_logFilePath = findPathByKey(defaultLogKey);
}

std::vector<std::string> TTM::CommandLineArgumentsParser::getAllParameters() const
{
	std::vector<std::string> parameters;
	parameters.reserve(3);
	parameters.push_back(delimiter + defaultInKey + " " + m_inFilePath);
	parameters.push_back(delimiter + defaultOutKey + " " + m_outFilePath);
	parameters.push_back(delimiter + defaultLogKey + " " + m_logFilePath);
	return parameters;
}

bool TTM::CommandLineArgumentsParser::optionExists(char** begin, char** end, std::string option)
{
	return std::find(begin, end, option) != end;
}

char* TTM::CommandLineArgumentsParser::getOption(char** begin, char** end, std::string option)
{
	char** itr = std::find(begin, end, option);

	if (itr != end && ++itr != end)
	{
		return *itr;
	}

	return nullptr;
}