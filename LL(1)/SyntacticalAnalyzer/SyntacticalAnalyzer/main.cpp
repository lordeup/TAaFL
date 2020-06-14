#include "SyntacticalAnalyzer.h"
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		std::cout << "The number of arguments does not match the task condition\n"
					 "Input should look: SyntacticalAnalyzer.exe <table input file> <sentence input file> <output file>\n";
		return 1;
	}

	std::ifstream fileTableInput(argv[1]);
	std::ifstream fileSentenceInput(argv[2]);
	std::ofstream fileOutput(argv[3]);

	if (!fileTableInput.is_open() || !fileSentenceInput.is_open())
	{
		std::cerr << "This file does not exist" << std::endl;
		return 1;
	}

	try
	{
		std::vector<InputTableData> inputTable;
		std::vector<OutputTableData> outputTable;
		std::string sentence;

		InitInputTable(fileTableInput, inputTable);
		InitSentence(fileSentenceInput, sentence);
		MakeProcess(inputTable, outputTable, sentence);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
