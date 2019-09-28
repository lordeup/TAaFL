#include "CAutomatController.h"
#include <fstream>
#include <iostream>

int main()
{
	std::ifstream fileInput(INPUT_FILE_NAME);
	std::ofstream fileOutput(OUTPUT_FILE_NAME);

	if (!fileInput.is_open())
	{
		std::cerr << ERROR_FILE_NOT_EXIST << std::endl;
		return 1;
	}

	CAutomatController automatController(fileInput, fileOutput);
	automatController.ProcessingCommand();

	return 0;
}
