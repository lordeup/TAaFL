#include "Logger.h"

Logger::Logger()
{
	m_outputFile = std::ofstream("Logger.txt");
}

void Logger::Log(std::string str)
{
	m_logger.push_back(str);
}

void Logger::Print()
{
	for (auto str : m_logger)
	{
		m_outputFile << str << std::endl;
	}
}