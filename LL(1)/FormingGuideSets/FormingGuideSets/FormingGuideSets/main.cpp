#include "GuideSets.h"
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "The number of arguments does not match the task condition\n"
					 "Input should look: FormingGuideSets.exe <input file> <output file>\n";
		return 1;
	}

	std::ifstream fileInput(argv[1]);
	std::ofstream fileOutput(argv[2]);

	if (!fileInput.is_open())
	{
		std::cerr << "This file does not exist" << std::endl;
		return 1;
	}

	std::srand(unsigned(std::time(0)));

	VectorString nonterminals;
	VectorString terminals;

	VectorInputData inputDatas;
	VectorOutputData outputDatas;

	FillingData(fileInput, inputDatas, nonterminals, terminals);
	Forming(inputDatas, outputDatas, nonterminals, terminals);
	AddingGuideCharacters(outputDatas, nonterminals, terminals);
	PrintResult(fileOutput, outputDatas);
}