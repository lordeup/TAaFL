#include "GeneratorLL1.h"

bool IsNonterminal(const std::string str)
{
	return str.front() == '<' && str.back() == '>';
}

std::string SubstrNonterminal(const std::string str)
{
	return str.substr(1, str.length() - 2);
}

void FillingData(std::ifstream& fileInput, std::vector<InputData>& inputDatas, std::vector<std::string>& nonterminals)
{
	std::string line;
	while (std::getline(fileInput, line))
	{
		std::istringstream iss(line);
		std::string str;

		InputData inputData;
		bool isTerminal = false;
		bool isGuideCharacter = false;

		while (iss >> str)
		{
			char firstCh = str.front();

			if (firstCh == '-')
			{
				isTerminal = true;
			}
			else if (firstCh == '/')
			{
				isGuideCharacter = true;
			}
			else if (isGuideCharacter)
			{
				inputData.guideCharacters.push_back(str);
			}
			else if (isTerminal)
			{
				inputData.terminals.push_back(str);
			}
			else
			{
				inputData.nonterminal = str;
				nonterminals.push_back(str);
			}
		}
		inputDatas.push_back(inputData);
	}
}

void Generate(const std::vector<InputData>& inputDatas, std::vector<OutputData>& outputDatas, const std::vector<std::string>& nonterminals)
{
	for (size_t i = 0; i < inputDatas.size(); ++i)
	{
		OutputData outputData;
		outputData.symbol = inputDatas[i].nonterminal;
		outputData.guideCharacters = inputDatas[i].guideCharacters;
		outputData.pointer = i == 0 ? inputDatas.size() + 1 : outputDatas.back().pointer + inputDatas[i - 1].terminals.size();

		outputDatas.push_back(outputData);
	}

	for (size_t i = 0; i < inputDatas.size(); ++i)
	{
		size_t terminalsSize = inputDatas[i].terminals.size();
		for (size_t j = 0; j < terminalsSize; ++j)
		{
			bool isNext = j + 1 < terminalsSize;

			OutputData outputData;
			outputData.symbol = inputDatas[i].terminals[j];
			outputData.guideCharacters = std::vector<std::string>{ outputData.symbol };
			outputData.isShift = true;
			outputData.pointer = outputDatas.size() + 2;

			if (!isNext)
			{
				outputData.pointer = 0;
			}

			if (outputData.symbol == END_SEQUENCE)
			{
				outputData.pointer = 0;
				outputData.isEnd = true;
			}
			else if (outputData.symbol == "e")
			{
				outputData.guideCharacters = std::vector<std::string>{ END_SEQUENCE };
				outputData.pointer = 0;
				outputData.isShift = false;
			}
			else
			{
				auto it = std::find(nonterminals.begin(), nonterminals.end(), outputData.symbol);

				if (it != nonterminals.end())
				{
					std::vector<std::string> characters;
					size_t distance = std::distance(nonterminals.begin(), it);
					size_t repeatCounter = std::count(nonterminals.begin(), nonterminals.end(), outputData.symbol);

					for (size_t k = 0; k < repeatCounter; ++k)
					{
						if (k < repeatCounter - 1)
						{
							outputDatas[distance + k].isError = false;
						}
						for (const auto& character : outputDatas[distance + k].guideCharacters)
						{
							characters.push_back(character);
						}
					}

					outputData.isShift = false;
					outputData.pointer = distance + 1;

					if (isNext)
					{
						outputData.isStack = true;
					}

					outputData.guideCharacters = characters;
				}
			}

			outputDatas.push_back(outputData);
		}
	}
}

void PrintInfoVector(std::ostream& fileOutput, const std::vector<std::string>& vec)
{
	if (!vec.empty())
	{
		std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<std::string>(fileOutput, " "));
		fileOutput << vec.back();
	}
}

void PrintResult(std::ostream& fileOutput, const std::vector<OutputData>& outputDatas)
{
	fileOutput << "Number" << TAB << "Symbol" << TAB << "Shift" << TAB << "Error" << TAB << "Pointer" << TAB << "Stack" << TAB << "End" << TAB << "Characters" << std::endl;

	for (size_t i = 0; i < outputDatas.size(); ++i)
	{
		OutputData outputData = outputDatas[i];
		size_t counter = i + 1;

		std::string symbol = IsNonterminal(outputData.symbol) ? SubstrNonterminal(outputData.symbol) : outputData.symbol;

		fileOutput << counter << TAB << symbol << TAB << outputData.isShift << TAB << outputData.isError << TAB << outputData.pointer << TAB << outputData.isStack << TAB << outputData.isEnd << TAB;
		PrintInfoVector(fileOutput, outputData.guideCharacters);
		fileOutput << std::endl;
	}
}
