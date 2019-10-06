#include "CAutomatController.h"
#include "Header.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << ERROR_NOT_ENOUGH_ARGUMENTS;
		return 1;
	}

	std::ifstream fileInput(argv[1]);
	//std::ofstream fileOutput(OUTPUT_FILE_NAME);

	if (!fileInput.is_open())
	{
		std::cerr << ERROR_FILE_NOT_EXIST << std::endl;
		return 1;
	}

	CAutomatController automatController(fileInput, std::cout);
	automatController.ProcessingCommand();

	return 0;
}