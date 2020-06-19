#include "GuideSets.h"

std::vector<OutputDataGuideSets> GetFormingGuideSets(std::istream& fileInput)
{
	std::vector<std::string> nonterminals;
	std::vector<std::string> terminals;

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
	std::vector<std::string> copyNonterminals;
	std::copy(nonterminals.begin(), nonterminals.end(), std::back_inserter(copyNonterminals));
	for (auto& nonterminal : copyNonterminals)
	{
		std::vector<InputData> temporaryVector;

		bool isBlankCharacter = false;

		for (auto it = inputDatas.begin(); it != inputDatas.end(); ++it)
		{
			it = std::find_if(it, inputDatas.end(), [&](const InputData& data) { return data.nonterminal == nonterminal; });
			if (it == inputDatas.end())
			{
				break;
			}
			temporaryVector.push_back(*it);
		}

		std::sort(temporaryVector.begin(), temporaryVector.end(), [](const InputData& a, const InputData& b) { return a.terminals.size() < b.terminals.size(); });
		auto it = std::find_if(temporaryVector.begin(), temporaryVector.end(), [&](const InputData& data) { return data.terminals.front() == NONTERMINAL_END_SEQUENCE; });

		if (it != temporaryVector.end())
		{
			isBlankCharacter = true;
			std::rotate(temporaryVector.begin(), it + 1, temporaryVector.end());
		}

		ActionsRightSide(inputDatas, outputDatas, nonterminals, terminals, nonterminal, temporaryVector, isBlankCharacter);
	}

	for (auto it = outputDatas.begin(); it != outputDatas.end(); ++it)
	{
		it = std::find_if(it, outputDatas.end(), [&](const OutputDataGuideSets& data) { return data.nonterminal == outputDatas.front().nonterminal; });
		if (it == outputDatas.end())
		{
			break;
		}

		if ((*it).terminals.back() != TERMINAL_END_SEQUENCE)
		{
			(*it).terminals.push_back(TERMINAL_END_SEQUENCE);
		}
	}
}

void ActionsRightSide(const std::vector<InputData>& inputDatas, std::vector<OutputDataGuideSets>& outputDatas, std::vector<std::string>& nonterminals, std::vector<std::string>& terminals,
	const std::string nonterminal, const std::vector<InputData>& temporaryVector, const bool isBlankCharacter)
{
	bool isLeftRecursion = false;
	bool isFactorization = false;

	for (size_t i = 0; i < temporaryVector.size(); ++i)
	{
		if (i < temporaryVector.size() - 1)
		{
			std::vector<std::string> first = temporaryVector.front().terminals;
			std::vector<std::string> second = temporaryVector[i + 1].terminals;

			isFactorization = std::all_of(first.begin(), first.end(), [&](const std::string str) { return std::find(second.begin(), second.end(), str) != second.end(); });
		}

		if (temporaryVector[i].terminals.front() == nonterminal)
		{
			isLeftRecursion = true;
		}
	}

	std::string randomNonterminal;

	while (true)
	{
		randomNonterminal = "<" + GetRandomString() + ">";

		if (IsCheckUniqueness(nonterminals, randomNonterminal))
		{
			break;
		}
	}

	if (isLeftRecursion || isFactorization)
	{
		nonterminals.push_back(randomNonterminal);
	}
	else if (temporaryVector.size() == 1)
	{
		size_t row = std::distance(inputDatas.begin(), std::find_if(inputDatas.begin(), inputDatas.end(), [&](const InputData& data) { return data.nonterminal == nonterminal; }));
		outputDatas.push_back({ inputDatas[row].nonterminal, inputDatas[row].terminals });
		return;
	}

	for (size_t i = 0; i < temporaryVector.size(); ++i)
	{
		InputData data = temporaryVector[i];
		OutputDataGuideSets out;

		if (isLeftRecursion)
		{
			data.terminals.push_back(randomNonterminal);

			if (isBlankCharacter && i == 0)
			{
				outputDatas.push_back({ nonterminal, std::vector<std::string>{ randomNonterminal } });
			}

			if (data.terminals.front() == nonterminal)
			{
				out.nonterminal = randomNonterminal;
				out.terminals = std::vector<std::string>(data.terminals.begin() + 1, data.terminals.end());
			}
			else if (!isBlankCharacter)
			{
				out.nonterminal = nonterminal;
				out.terminals = data.terminals;
			}
		}
		else if (isFactorization)
		{
			if (i == 0)
			{
				data.terminals.push_back(randomNonterminal);
				out.nonterminal = nonterminal;
				out.terminals = data.terminals;
			}
			else
			{
				out.nonterminal = randomNonterminal;
				out.terminals = std::vector<std::string>(data.terminals.begin() + temporaryVector.front().terminals.size(), data.terminals.end());
			}
		}
		else
		{
			out.nonterminal = nonterminal;
			out.terminals = data.terminals;
		}

		if (!out.nonterminal.empty())
		{
			outputDatas.push_back(out);
		}
	}

	if (isLeftRecursion || isFactorization)
	{
		if (IsCheckUniqueness(terminals, NONTERMINAL_END_SEQUENCE))
		{
			terminals.push_back(NONTERMINAL_END_SEQUENCE);
		}

		outputDatas.push_back({ randomNonterminal, std::vector<std::string>{ NONTERMINAL_END_SEQUENCE } });
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

			for (size_t i = nonterminals.size(); i < transitions[row].second.size(); ++i)
			{
				if (transitions[row].second[i].second)
				{
					std::string str = transitions[row].second[i].first;

					if (!str.empty() && (IsNonterminal(firstTerminal) || firstTerminal == str || firstTerminal == NONTERMINAL_END_SEQUENCE))
					{
						guideCharacters.push_back(str == NONTERMINAL_END_SEQUENCE ? TERMINAL_END_SEQUENCE : str);
						transitions[row].second[i].first = "";
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
