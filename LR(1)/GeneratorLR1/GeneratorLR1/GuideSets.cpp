#include "GuideSets.h"

std::vector<OutputDataGuideSets> GetFormingGuideSets(std::istream& fileInput, std::vector<std::string>& nonterminals, std::vector<std::string>& terminals)
{
	std::vector<InputData> inputDatas;
	std::vector<OutputDataGuideSets> outputDatas;

	FillingData(fileInput, inputDatas, nonterminals, terminals);
	Forming(inputDatas, outputDatas, nonterminals, terminals);
	AddingGuideCharacters(outputDatas, nonterminals, terminals);

	return outputDatas;
}

void FillingData(std::istream& fileInput, std::vector<InputData>& inputDatas, std::vector<std::string>& nonterminals, std::vector<std::string>& terminals)
{
	std::string line;
	while (std::getline(fileInput, line))
	{
		std::istringstream iss(line);
		std::string str;

		InputData inputData;
		bool isTerminal = false;

		while (iss >> str)
		{
			if (str.front() == '-')
			{
				isTerminal = true;
			}
			else if (isTerminal)
			{
				inputData.terminals.push_back(str);

				if (!IsNonterminal(str) && IsCheckUniqueness(terminals, str))
				{
					bool isEndSequence = str == NONTERMINAL_END_SEQUENCE || str == TERMINAL_END_SEQUENCE;
					terminals.push_back(isEndSequence ? NONTERMINAL_END_SEQUENCE : str);
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

void Forming(const std::vector<InputData>& inputDatas, std::vector<OutputDataGuideSets>& outputDatas, std::vector<std::string>& nonterminals, std::vector<std::string>& terminals)
{
	for (auto& nonterminal : nonterminals)
	{
		std::vector<InputData> temporaryVector;

		for (auto it = inputDatas.begin(); it != inputDatas.end(); ++it)
		{
			it = std::find_if(it, inputDatas.end(), [&](const InputData& data) { return data.nonterminal == nonterminal; });
			if (it == inputDatas.end())
			{
				break;
			}
			temporaryVector.push_back(*it);
		}

		std::sort(temporaryVector.begin(), temporaryVector.end(), [](const InputData& a, const InputData& b) { return a.terminals.front() != b.terminals.front() && a.terminals.front() == a.nonterminal; });
		std::for_each(temporaryVector.begin(), temporaryVector.end(), [&](const InputData& data) { outputDatas.push_back({ data.nonterminal, data.terminals }); });
	}

	size_t count = std::count_if(outputDatas.begin(), outputDatas.end(), [&](const OutputDataGuideSets& data) { return data.nonterminal == outputDatas.front().nonterminal; });

	if (count > 1)
	{
		std::string randomNonterminal;

		while (true)
		{
			randomNonterminal = "<" + GetRandomString() + ">";

			if (IsCheckUniqueness(nonterminals, randomNonterminal))
			{
				break;
			}
		}

		nonterminals.insert(nonterminals.begin(), randomNonterminal);

		outputDatas.insert(outputDatas.begin(), { randomNonterminal, std::vector<std::string>{ outputDatas.front().nonterminal } });
	}

	if (outputDatas.front().terminals.back() != TERMINAL_END_SEQUENCE)
	{
		if (IsCheckUniqueness(terminals, NONTERMINAL_END_SEQUENCE))
		{
			terminals.push_back(NONTERMINAL_END_SEQUENCE);
		}

		outputDatas.front().terminals.push_back(TERMINAL_END_SEQUENCE);
	}
}

void AddingGuideCharacters(std::vector<OutputDataGuideSets>& outputDatas, const std::vector<std::string>& nonterminals, const std::vector<std::string>& terminals)
{
	std::vector<PairStringVectorPair> transitions;
	std::vector<PairStringBool> characters;

	std::for_each(nonterminals.begin(), nonterminals.end(), [&](const std::string str) { characters.push_back(std::make_pair(str, false)); });
	std::for_each(terminals.begin(), terminals.end(), [&](const std::string str) { characters.push_back(std::make_pair(str, false)); });

	std::for_each(nonterminals.begin(), nonterminals.end(), [&](const std::string str) { transitions.push_back(std::make_pair(str, characters)); });

	BuildingFirstRelationship(outputDatas, transitions, characters);
	BuildingFirstPlusRelationship(transitions);

	for (auto& outputData : outputDatas)
	{
		size_t row = std::distance(transitions.begin(), GetIteratorFindIfVector(transitions, outputData.nonterminal));

		if (row < transitions.size())
		{
			std::vector<std::string> guideCharacters;
			std::string firstTerminal = outputData.terminals.front();

			for (auto& transitionSecond : transitions[row].second)
			{
				if (transitionSecond.second && !transitionSecond.first.empty())
				{
					if (outputData.nonterminal == transitionSecond.first)
					{
						guideCharacters.push_back(transitionSecond.first);
						transitionSecond.first = "";
						break;
					}

					if (outputData.nonterminal != firstTerminal && IsNonterminal(firstTerminal) || firstTerminal == transitionSecond.first || firstTerminal == NONTERMINAL_END_SEQUENCE)
					{
						guideCharacters.push_back(transitionSecond.first == NONTERMINAL_END_SEQUENCE ? TERMINAL_END_SEQUENCE : transitionSecond.first);
						transitionSecond.first = "";
					}
				}
			}
			outputData.guideCharacters = guideCharacters;
		}
	}
}

void BuildingFirstRelationship(std::vector<OutputDataGuideSets>& outputDatas, std::vector<PairStringVectorPair>& transitions, std::vector<PairStringBool>& characters)
{
	for (auto& outputData : outputDatas)
	{
		size_t row = std::distance(transitions.begin(), GetIteratorFindIfVector(transitions, outputData.nonterminal));
		size_t column = std::distance(characters.begin(), GetIteratorFindIfVector(characters, outputData.terminals.front()));

		if (row < transitions.size() && column < characters.size())
		{
			if (outputData.terminals.front() == NONTERMINAL_END_SEQUENCE)
			{
				SearchStartingTerminalsEmptyRules(outputDatas, outputData.nonterminal, outputData.nonterminal, transitions, characters);
			}
			else
			{
				transitions[row].second[column].second = true;
			}
		}
	}
}

void SearchStartingTerminalsEmptyRules(std::vector<OutputDataGuideSets>& outputDatas, const std::string parentNonterminal, const std::string nonterminal,
	std::vector<PairStringVectorPair>& transitions, std::vector<PairStringBool>& characters)
{
	for (const auto& outputData : outputDatas)
	{
		auto it1 = std::find_if(outputData.terminals.begin(), outputData.terminals.end(), [&](const std::string& str) { return str == nonterminal; });

		if (it1 != outputData.terminals.end() && outputData.nonterminal != nonterminal)
		{
			size_t distance = std::distance(outputData.terminals.begin(), it1);
			size_t size = outputData.terminals.size() - 1;
			std::string terminal = distance <= size ? (distance < size ? outputData.terminals[distance + 1] : outputData.terminals.back()) : NONTERMINAL_END_SEQUENCE;

			size_t row = std::distance(transitions.begin(), GetIteratorFindIfVector(transitions, parentNonterminal));
			size_t column = std::distance(characters.begin(), GetIteratorFindIfVector(characters, terminal == TERMINAL_END_SEQUENCE ? NONTERMINAL_END_SEQUENCE : terminal));

			if (row < transitions.size() && column < characters.size() && terminal != parentNonterminal)
			{
				transitions[row].second[column].second = true;
			}

			if (nonterminal == parentNonterminal)
			{
				for (auto& data : outputDatas)
				{
					auto it2 = std::find_if(data.terminals.begin(), data.terminals.end(), [&](const std::string& str) { return str == outputData.nonterminal; });

					if (it2 != data.terminals.end())
					{
						SearchStartingTerminalsEmptyRules(outputDatas, parentNonterminal, (*it2), transitions, characters);
					}
				}
			}
		}
	}
}

void BuildingFirstPlusRelationship(std::vector<PairStringVectorPair>& transitions)
{
	for (auto it = transitions.rbegin(); it != transitions.rend(); ++it)
	{
		for (size_t j = 0; j < transitions.size(); ++j)
		{
			if ((*it).second[j].second)
			{
				size_t row = std::distance(transitions.begin(), GetIteratorFindIfVector(transitions, (*it).second[j].first));

				for (size_t k = 0; k < transitions[row].second.size(); ++k)
				{
					if (transitions[row].second[k].second)
					{
						(*it).second[k].second = true;
					}
				}
			}
		}
	}
}

void PrintResultGuideSets(std::ofstream& fileOutput, const std::vector<OutputDataGuideSets>& outputDatas)
{
	for (const auto& outputData : outputDatas)
	{
		fileOutput << outputData.nonterminal << SPACE << "-" << SPACE;
		PrintInfoVector(fileOutput, outputData.terminals, SPACE);
		fileOutput << SPACE << "/" << SPACE;
		PrintInfoVector(fileOutput, outputData.guideCharacters, SPACE);
		fileOutput << std::endl;
	}
}