#pragma once
#include "Header.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>

struct InputData
{
	std::string nonterminal;
	VectorString terminals;
	VectorString guideCharacters;
};

struct OutputData
{
	std::string symbol;
	VectorString guideCharacters;
	bool isShift = false;
	bool isError = true;
	size_t pointer = 0;
	bool isStack = false;
	bool isEnd = false;
};

class GeneratorLL1
{
public:
	GeneratorLL1(std::istream& input, std::ostream& output);

	void Execute();

private:
	std::istream& m_input;
	std::ostream& m_output;

	VectorString m_nonterminals;
	std::vector<InputData> m_inputData;
	std::vector<OutputData> m_outputData;

	void PrintInfoVector(const VectorString& vec) const;

	void FillingData();
	void Generate();
	void PrintResult();
};