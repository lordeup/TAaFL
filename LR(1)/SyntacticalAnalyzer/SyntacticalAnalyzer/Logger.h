#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class Logger
{
public:
	Logger();
	void Log(std::string str);
	void Print();

private:
	std::ofstream m_outputFile;
	std::vector<std::string> m_logger;
};
