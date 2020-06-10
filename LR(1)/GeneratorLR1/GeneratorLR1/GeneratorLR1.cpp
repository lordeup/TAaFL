#include "GeneratorLR1.h"

bool IsNonterminal(const std::string str)
{
	return !str.empty() && str.front() == '<' && str.back() == '>';
}

bool IsCheckUniqueness(const VectorString& vec, const std::string str)
{
	return std::find(vec.begin(), vec.end(), str) == vec.end();
}

void FillingData(std::istream& fileInput, std::vector<InputData>& inputDatas, VectorString& nonterminals, VectorString& terminals)
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
			if (str.front() == '-')
			{
				isTerminal = true;
			}
			else if (str.front() == '/')
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

				if (!IsNonterminal(str) && IsCheckUniqueness(terminals, str))
				{
					bool isEndSequence = str == NONTERMINAL_END_SEQUENCE || str == TERMINAL_END_SEQUENCE;
					terminals.push_back(isEndSequence ? TERMINAL_END_SEQUENCE : str);
				}
			}
			else
			{
				inputData.nonterminal = str;

				if (IsCheckUniqueness(nonterminals, str))
				{
					nonterminals.push_back(str);
				}
			}
		}
		inputDatas.push_back(inputData);
	}
}

void Generate(const std::vector<InputData>& inputDatas, std::vector<VectorString>& outputDatas, const VectorString& nonterminals, const VectorString& terminals, VectorString& characters)
{
	std::for_each(nonterminals.begin(), nonterminals.end(), [&](const std::string str) { characters.push_back(str); });
	std::for_each(terminals.begin(), terminals.end(), [&](const std::string str) { characters.push_back(str); });

	std::queue<std::string> queueCurrentCharacter;
	size_t count = 2;

	VectorString temporaryVector(characters.size());
	temporaryVector.front() = OK;
	std::for_each(inputDatas.front().guideCharacters.begin(), inputDatas.front().guideCharacters.end(), [&](const std::string str) { AddShift(temporaryVector, characters, queueCurrentCharacter, str, count); });
	outputDatas.push_back(temporaryVector);

	while (!queueCurrentCharacter.empty())
	{
		VectorString vec(characters.size());

		for (size_t i = 0; i < inputDatas.size(); ++i)
		{
			const InputData inputData = inputDatas[i];

			auto it = std::find_if(inputData.terminals.begin(), inputData.terminals.end(), [&](const std::string& str) { return str == queueCurrentCharacter.front(); });

			if (it != inputData.terminals.end())
			{
				size_t nextIndex = std::distance(inputData.terminals.begin(), it) + 1;

				if (nextIndex < inputData.terminals.size() && inputData.terminals[nextIndex] != TERMINAL_END_SEQUENCE)
				{
					if (IsNonterminal(inputData.terminals[nextIndex]))
					{
						for (auto it = inputDatas.begin(); it != inputDatas.end(); ++it)
						{
							it = std::find_if(it, inputDatas.end(), [&](const InputData& data) { return data.nonterminal == inputData.terminals[nextIndex]; });
							if (it == inputDatas.end())
							{
								break;
							}

							std::for_each((*it).guideCharacters.begin(), (*it).guideCharacters.end(), [&](const std::string str) { AddShift(vec, characters, queueCurrentCharacter, str, count); });
						}
					}
					else
					{
						AddShift(vec, characters, queueCurrentCharacter, inputData.terminals[nextIndex], count);
					}
				}
				else
				{
					size_t row = GetDistanceVector(characters, TERMINAL_END_SEQUENCE);

					if (row < characters.size())
					{
						vec[row] = ROLL_UP + std::to_string(i + 1);
					}
				}
			}
		}

		queueCurrentCharacter.pop();

		outputDatas.push_back(vec);
	}
}

void AddShift(VectorString& temporaryVector, VectorString& characters, std::queue<std::string>& queue, const std::string str, size_t& count)
{
	size_t row = GetDistanceVector(characters, str);

	if (row < characters.size())
	{
		temporaryVector[row] = SHIFT + std::to_string(count);
		queue.push(str);
		++count;
	}
}

void PrintInfoVector(std::ofstream& fileOutput, const std::vector<std::string>& vec)
{
	if (!vec.empty())
	{
		std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<std::string>(fileOutput, "\t"));
		fileOutput << vec.back();
	}
}

void PrintResult(std::ofstream& fileOutput, const std::vector<VectorString>& outputDatas, const VectorString& characters)
{
	fileOutput << "№" << TAB;
	PrintInfoVector(fileOutput, characters);

	fileOutput << std::endl;

	for (size_t i = 0; i < outputDatas.size(); ++i)
	{
		fileOutput << i + 1 << TAB;

		for (const auto& str : outputDatas[i])
		{
			fileOutput << (str.empty() ? "-" : str) << TAB;
		}

		fileOutput << std::endl;
	}
}

size_t GetDistanceVector(VectorString& vec, const std::string str)
{
	return std::distance(vec.begin(), std::find_if(vec.begin(), vec.end(), [&](const std::string data) { return data == str; }));
}