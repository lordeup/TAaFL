#include "GeneratorLL1.h"

std::vector<OutputData> GetGenerateData(const std::vector<OutputDataGuideSets>& inputDatas)
{
	std::vector<OutputData> outputDatas;

	for (size_t i = 0; i < inputDatas.size(); ++i)
	{
		OutputData outputData;
		outputData.symbol = inputDatas[i].nonterminal;
		outputData.guideCharacters = inputDatas[i].guideCharacters;

		if (inputDatas[i].terminals.front() == NONTERMINAL_END_SEQUENCE && IsCheckUniqueness(outputData.guideCharacters, TERMINAL_END_SEQUENCE))
		{
			outputData.guideCharacters.push_back(TERMINAL_END_SEQUENCE);
		}

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

			if (outputData.symbol == TERMINAL_END_SEQUENCE)
			{
				outputData.pointer = 0;
				outputData.isEnd = true;
			}
			else if (outputData.symbol == NONTERMINAL_END_SEQUENCE)
			{
				outputData.guideCharacters = inputDatas[i].guideCharacters;
				outputData.pointer = 0;
				outputData.isShift = false;
			}
			else
			{
				size_t row = std::distance(inputDatas.begin(),
					std::find_if(inputDatas.begin(), inputDatas.end(), [&](const OutputDataGuideSets& data) { return data.nonterminal == outputData.symbol; }));

				if (row < inputDatas.size())
				{
					std::vector<std::string> characters;
					size_t repeatCounter = std::count_if(inputDatas.begin(), inputDatas.end(), [&](const OutputDataGuideSets& data) { return data.nonterminal == outputData.symbol; });

					for (size_t k = 0; k < repeatCounter; ++k)
					{
						if (k < repeatCounter - 1)
						{
							outputDatas[row + k].isError = false;
						}
						for (const auto& character : outputDatas[row + k].guideCharacters)
						{
							characters.push_back(character);
						}
					}

					outputData.isShift = false;
					outputData.pointer = row + 1;

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

	return outputDatas;
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
		PrintInfoVector(fileOutput, outputData.guideCharacters, SPACE);
		fileOutput << std::endl;
	}
}
