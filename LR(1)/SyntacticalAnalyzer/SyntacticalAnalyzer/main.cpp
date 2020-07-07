#include "LRReader.h"
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

	try
	{
		std::ifstream fileTableInput(argv[1]);
		std::ifstream fileSentenceInput(argv[2]);
		std::ofstream fileOutput(argv[3]);

		if (!fileTableInput.is_open() || !fileSentenceInput.is_open())
		{
			std::cerr << "This file does not exist" << std::endl;
			return 1;
		}

		LRReader lrReader;
		lrReader.ReadSentence(fileSentenceInput);
		lrReader.ReadTable(fileTableInput);

		auto headerSymbols = lrReader.GetHeaderSymbols();
		auto lrData = lrReader.GetLRData();
		auto sentence = lrReader.GetSentence();

		SyntacticalAnalyzer analyzer(headerSymbols, lrData, sentence);
		analyzer.Run();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
