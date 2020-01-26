#include "Lexer.h"
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << ERROR_NOT_ENOUGH_ARGUMENTS;
		return 1;
	}

	std::ifstream fileInput(argv[1]);
	std::ofstream fileOutput(argv[2]);

	if (!fileInput.is_open())
	{
		std::cerr << ERROR_FILE_NOT_EXIST << std::endl;
		return 1;
	}

	Lexer lexer(fileInput, fileOutput);
	lexer.Run();
}