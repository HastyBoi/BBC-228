#include "pch.h"
#include "CommandLineArgumentsParser.h"
#include "Error.h"

TTM::CommandLineArgumentsParser::CommandLineArgumentsParser(int argc, char** argv)
{
	if (argc <= 1 || getOption(argv + 1, argv + argc, delimiter + inKey) == nullptr)
		throw ERROR_THROW(100);

	auto findPathByKey = [&](const std::string& key)
	{
		char* path = getOption(argv + 1, argv + argc, delimiter + key);
		return (path == nullptr) ? m_inFilePath + '.' + key : path;
	};

	m_inFilePath = findPathByKey(inKey);
	m_outFilePath = findPathByKey(outKey);
	m_logFilePath = findPathByKey(logKey);

	if (optionExists(argv + 1, argv + argc, delimiter + lexKey))
	{
		m_lexTablePath = m_inFilePath + '.' + lexKey + ".txt";
	}
	if (optionExists(argv + 1, argv + argc, delimiter + idKey))
	{
		m_idTablePath = m_inFilePath + '.' + idKey + ".txt";
	}
}

std::vector<std::string> TTM::CommandLineArgumentsParser::getAllParameters() const
{
	std::vector<std::string> parameters;
	parameters.reserve(5);
	parameters.push_back(delimiter + inKey + " " + m_inFilePath);
	parameters.push_back(delimiter + outKey + " " + m_outFilePath);
	parameters.push_back(delimiter + logKey + " " + m_logFilePath);
	if (!m_lexTablePath.empty())
	{
		parameters.push_back(delimiter + lexKey + " " + m_lexTablePath);
	}
	if (!m_idTablePath.empty())
	{
		parameters.push_back(delimiter + idKey + " " + m_idTablePath);
	}
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