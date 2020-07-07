#include "LRReader.h"

LRReader::LRReader()
{
}

void LRReader::ReadSentence(std::ifstream& fileSentenceInput)
{
	std::string line, str;
	std::getline(fileSentenceInput, line);
	std::istringstream iss(line);
	while (iss >> str)
	{
		m_sentence.push_back(str);
	}
}

void LRReader::ReadTable(std::ifstream& fileTableInput)
{
	std::string line;

	std::getline(fileTableInput, line);
	InitHeaderSymbols(line);

	while (std::getline(fileTableInput, line))
	{
		std::istringstream iss(line);

		LRData lrData;

		lrData.number = ParseNumber(GetString(iss));
		lrData.rule = GetString(iss);
		lrData.ch = GetString(iss);
		lrData.size = ParseNumber(GetString(iss));
		lrData.symbols = GetSymbols(iss);

		m_lrData.push_back(lrData);
	}
}

std::vector<std::string> LRReader::GetHeaderSymbols()
{
	return m_headerSymbols;
}

std::vector<LRData> LRReader::GetLRData()
{
	return m_lrData;
}

std::list<std::string> LRReader::GetSentence()
{
	return m_sentence;
}

void LRReader::InitHeaderSymbols(std::string line)
{
	std::istringstream iss(line);
	std::string str;
	for (size_t i = 0; i <= 3; i++)
	{
		iss >> str;
	}
	while (iss >> str)
	{
		m_headerSymbols.push_back(str);
	}
}

std::vector<Symbol> LRReader::GetSymbols(std::istringstream& iss)
{
	std::vector<Symbol> symbols;
	std::string str;
	while (iss >> str)
	{
		Symbol symbol = ParseSymbol(str);
		symbols.push_back(symbol);
	}

	return symbols;
}
