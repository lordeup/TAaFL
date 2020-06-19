#include "GeneratorLR1.h"

std::vector<std::string> GetCharacters(const std::vector<std::string>& nonterminals, const std::vector<std::string>& terminals)
{
	std::vector<std::string> characters;

	std::for_each(nonterminals.begin(), nonterminals.end(), [&](const std::string str) { characters.push_back(str); });
	std::for_each(terminals.begin(), terminals.end(), [&](const std::string str) { characters.push_back(str); });

	return characters;
}

std::vector<std::vector<std::string>> GetGenerateData(const std::vector<OutputDataGuideSets>& inputDatas, const std::vector<std::string>& characters)
{
	std::vector<std::vector<std::string>> outputDatas;

	std::vector<TableData> rightSides = GetRightSides(inputDatas);
	std::vector<TableData> guideCharacters = GetGuideCharacters(inputDatas, rightSides);
	std::vector<TableData> states;

	std::stack<TableData> stackTableData;

	size_t count = 2;

	std::vector<std::string> temporaryVector(characters.size());
	temporaryVector.front() = OK;

	for (auto it = guideCharacters.begin(); it != guideCharacters.end(); ++it)
	{
		it = std::find_if(it, guideCharacters.end(), [&](const TableData& data) { return data.nonterminal == inputDatas.front().nonterminal; });
		if (it == guideCharacters.end())
		{
			break;
		}

		AddShift(*it, count, characters, temporaryVector, states, stackTableData);
	}

	outputDatas.push_back(temporaryVector);

	while (!stackTableData.empty())
	{
		temporaryVector.clear();
		temporaryVector.resize(characters.size());

		TableData topData = stackTableData.top();

		size_t nextIndex = topData.position + 1;
		std::string nextStr = nextIndex < inputDatas[topData.row].terminals.size() ? inputDatas[topData.row].terminals[nextIndex] : "";

		if (!nextStr.empty() && nextStr != TERMINAL_END_SEQUENCE)
		{
			TableData tableData{ "", nextStr, topData.row, nextIndex };

			AddShift(tableData, count, characters, temporaryVector, states, stackTableData);

			if (IsNonterminal(nextStr))
			{
				for (auto it = guideCharacters.begin(); it != guideCharacters.end(); ++it)
				{
					it = std::find_if(it, guideCharacters.end(), [&](const TableData& data) { return data.nonterminal == nextStr; });
					if (it == guideCharacters.end())
					{
						break;
					}

					AddShift(*it, count, characters, temporaryVector, states, stackTableData);
				}
			}
		}
		else
		{
			if (nextStr == TERMINAL_END_SEQUENCE)
			{
				AddRollUp(nextStr, topData.row, characters, temporaryVector);
			}
			else
			{
				FindRollUp(inputDatas[topData.row].nonterminal, topData.row, rightSides, inputDatas, characters, temporaryVector);
			}
		}

		stackTableData.pop();
		outputDatas.push_back(temporaryVector);
	}

	return outputDatas;
}

void AddShift(const TableData& tableData, size_t& count, const std::vector<std::string>& characters, std::vector<std::string>& vec, std::vector<TableData>& states, std::stack<TableData>& stack)
{
	size_t row = GetDistanceVector(characters, tableData.character);

	if (row < characters.size())
	{
		vec[row] = SHIFT + std::to_string(count);

		if (IsCheckTableDataUniqueness(states, tableData))
		{
			states.push_back(tableData);
			stack.push(tableData);
		}

		++count;
	}
}

std::vector<TableData> GetRightSides(const std::vector<OutputDataGuideSets>& inputDatas)
{
	std::vector<TableData> vec;

	for (size_t i = 0; i < inputDatas.size(); ++i)
	{
		for (size_t j = 0; j < inputDatas[i].terminals.size(); ++j)
		{
			vec.push_back(TableData{ inputDatas[i].nonterminal, inputDatas[i].terminals[j], i, j });
		}
	}

	return vec;
}

void RecordingNonterminalGuideSets(const std::string str, const std::string nonterminal, const std::vector<TableData>& rightSides, std::vector<TableData>& tableDataGuideCharacters, std::vector<std::string>& copyGuideCharacters)
{
	if (copyGuideCharacters.empty() || str.empty())
	{
		return;
	}

	std::string newStr;

	for (auto it = rightSides.begin(); it != rightSides.end(); ++it)
	{
		it = std::find_if(it, rightSides.end(), [&](const TableData& data) { return data.nonterminal == str && data.nonterminal != data.character && data.position == 0; });
		if (it == rightSides.end())
		{
			break;
		}

		if ((*it).character != NONTERMINAL_END_SEQUENCE)
		{
			if (IsNonterminal((*it).character))
			{
				newStr = (*it).character;
			}

			tableDataGuideCharacters.push_back(TableData{ nonterminal, (*it).character, (*it).row });
			copyGuideCharacters.erase(std::remove(copyGuideCharacters.begin(), copyGuideCharacters.end(), (*it).character), copyGuideCharacters.end());
		}
	}

	RecordingNonterminalGuideSets(newStr, nonterminal, rightSides, tableDataGuideCharacters, copyGuideCharacters);
}

std::vector<TableData> GetGuideCharacters(const std::vector<OutputDataGuideSets>& inputDatas, const std::vector<TableData>& rightSides)
{
	std::vector<TableData> vec;

	for (size_t i = 0; i < inputDatas.size(); ++i)
	{
		std::vector<std::string> copyGuideCharacters;
		std::copy(inputDatas[i].guideCharacters.begin(), inputDatas[i].guideCharacters.end(), std::back_inserter(copyGuideCharacters));

		for (size_t j = 0; j < inputDatas[i].guideCharacters.size() && !copyGuideCharacters.empty(); ++j)
		{
			std::string str = inputDatas[i].guideCharacters[j];

			vec.push_back(TableData{ inputDatas[i].nonterminal, str, i });
			copyGuideCharacters.erase(std::remove(copyGuideCharacters.begin(), copyGuideCharacters.end(), str), copyGuideCharacters.end());

			if (IsNonterminal(str))
			{
				RecordingNonterminalGuideSets(str, inputDatas[i].nonterminal, rightSides, vec, copyGuideCharacters);
			}
		}
	}

	return vec;
}

void AddRollUp(const std::string str, const size_t index, const std::vector<std::string>& characters, std::vector<std::string>& vec)
{
	size_t row = GetDistanceVector(characters, str);

	if (row < characters.size() && vec[row].empty())
	{
		vec[row] = ROLL_UP + std::to_string(index + 1);
	}
}

void FindRollUp(const std::string nonterminal, const size_t index, const std::vector<TableData>& rightSides, const std::vector<OutputDataGuideSets>& inputDatas, const std::vector<std::string>& characters, std::vector<std::string>& vec)
{
	for (auto it = rightSides.begin(); it != rightSides.end(); ++it)
	{
		it = std::find_if(it, rightSides.end(), [&](const TableData& data) { return data.character == nonterminal; });

		if (it == rightSides.end())
		{
			break;
		}

		size_t nextIndex = (*it).position + 1;

		if (nextIndex < inputDatas[(*it).row].terminals.size())
		{
			AddRollUp(inputDatas[(*it).row].terminals[nextIndex], index, characters, vec);
		}
		else
		{
			FindRollUp(inputDatas[(*it).row].nonterminal, index, rightSides, inputDatas, characters, vec);
		}
	}
}

void PrintResult(std::ofstream& fileOutput, const std::vector<std::vector<std::string>>& outputDatas, const std::vector<std::string>& characters)
{
	fileOutput << "Number" << TAB;
	PrintInfoVector(fileOutput, characters, TAB);

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
