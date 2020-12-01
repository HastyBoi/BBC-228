#include "pch.h"
#include "Parm.h"
#include "Error.h"

Parm::Parameters::Parameters(int argc, char** argv)
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

std::vector<std::string> Parm::Parameters::getAllParameters() const
{
	std::vector<std::string> parameters;
	parameters.reserve(3);
	parameters.push_back(delimiter + defaultInKey + " " + m_inFilePath);
	parameters.push_back(delimiter + defaultOutKey + " " + m_outFilePath);
	parameters.push_back(delimiter + defaultLogKey + " " + m_logFilePath);
	return parameters;
}

bool Parm::Parameters::optionExists(char** begin, char** end, std::string_view option)
{
	return std::find(begin, end, option) != end;
}

char* Parm::Parameters::getOption(char** begin, char** end, std::string_view option)
{
	char** itr = std::find(begin, end, option);

	if (itr != end && ++itr != end)
	{
		return *itr;
	}

	return nullptr;
}