#include "GuideSets.h"

GuideSets::GuideSets(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
	std::srand(unsigned(std::time(0)));
}

void GuideSets::Execute()
{
	FillingData();
	Forming();
	AddingGuideCharacters();
	PrintResult();
}

void GuideSets::FillingData()
{
	std::string line;
	while (std::getline(m_input, line))
	{
		std::istringstream iss(line);
		std::string str;

		InputData inputData;
		bool isRightSide = false;

		while (iss >> str)
		{
			if (IsNonterminal(str))
			{
				if (isRightSide)
				{
					inputData.terminals.push_back(str);
				}
				else
				{
					inputData.nonterminal = str;
					m_nonterminals.push_back(str);
				}
			}
			else if (str.front() == '-')
			{
				isRightSide = true;
			}
			else
			{
				inputData.terminals.push_back(str);
				bool isEndSequence = str == NONTERMINAL_END_SEQUENCE || str == TERMINAL_END_SEQUENCE;
				m_terminals.push_back(isEndSequence ? NONTERMINAL_END_SEQUENCE : str);
			}
		}
		m_inputData.push_back(inputData);
	}
}

void GuideSets::Forming()
{
	m_nonterminals = GetUniques(m_nonterminals);

	std::vector<std::string> copyNonterminals;
	std::copy(m_nonterminals.begin(), m_nonterminals.end(), std::back_inserter(copyNonterminals));

	for (auto& nonterminal : copyNonterminals)
	{
		std::vector<InputData> temporaryVector;

		for (auto it = m_inputData.begin(); it != m_inputData.end(); ++it)
		{
			it = std::find_if(it, m_inputData.end(), [&](const InputData& data) { return data.nonterminal == nonterminal; });
			if (it == m_inputData.end())
			{
				break;
			}
			temporaryVector.push_back(*it);
		}

		size_t row = std::distance(m_inputData.begin(), std::find_if(m_inputData.begin(), m_inputData.end(), [&](const InputData& data) { return data.nonterminal == nonterminal; }));

		if (temporaryVector.size() == 1)
		{
			OutputData outputData;
			outputData.nonterminal = m_inputData[row].nonterminal;
			outputData.terminals = m_inputData[row].terminals;
			m_outputData.push_back(outputData);
		}
		else
		{
			std::sort(temporaryVector.begin(), temporaryVector.end(), [](const InputData& a, const InputData& b) { return a.terminals.size() < b.terminals.size(); });
			auto it = std::find_if(temporaryVector.begin(), temporaryVector.end(), [&](const InputData& data) { return data.terminals.front() == NONTERMINAL_END_SEQUENCE; });

			if (it != temporaryVector.end())
			{
				std::rotate(temporaryVector.begin(), it + 1, temporaryVector.end());
			}

			ActionsRightSide(nonterminal, temporaryVector);
		}
	}

	for (auto it = m_outputData.begin(); it != m_outputData.end(); ++it)
	{
		it = std::find_if(it, m_outputData.end(), [&](const OutputData& data) { return data.nonterminal == m_outputData.front().nonterminal; });
		if (it == m_outputData.end())
		{
			break;
		}

		if ((*it).terminals.back() != TERMINAL_END_SEQUENCE)
		{
			(*it).terminals.push_back(TERMINAL_END_SEQUENCE);
		}
	}
}

void GuideSets::ActionsRightSide(const std::string nonterminal, const std::vector<InputData>& temporaryVector)
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

	std::string randomNonterminal = "<" + GetRandomNonterminal() + ">";

	if (isLeftRecursion || isFactorization)
	{
		m_nonterminals.push_back(randomNonterminal);
	}

	for (size_t i = 0; i < temporaryVector.size(); ++i)
	{
		InputData data = temporaryVector[i];
		OutputData out;

		if (isLeftRecursion)
		{
			data.terminals.push_back(randomNonterminal);
			if (data.terminals.front() == nonterminal)
			{
				out.nonterminal = randomNonterminal;
				out.terminals = std::vector<std::string>(data.terminals.begin() + 1, data.terminals.end());
			}
			else
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

		m_outputData.push_back(out);
	}

	if (isLeftRecursion || isFactorization)
	{
		OutputData outputData;
		outputData.nonterminal = randomNonterminal;
		outputData.terminals = std::vector<std::string>{ NONTERMINAL_END_SEQUENCE };
		m_terminals.push_back(NONTERMINAL_END_SEQUENCE);
		m_outputData.push_back(outputData);
	}
}

void GuideSets::AddingGuideCharacters()
{
	std::vector<PairStringVectorPair> transitions;
	std::vector<PairStringBool> ñharacters;

	m_terminals = GetUniques(m_terminals);

	std::for_each(m_nonterminals.begin(), m_nonterminals.end(), [&](const std::string str) { ñharacters.push_back(std::make_pair(str, false)); });
	std::for_each(m_terminals.begin(), m_terminals.end(), [&](const std::string str) { ñharacters.push_back(std::make_pair(str, false)); });

	std::for_each(m_nonterminals.begin(), m_nonterminals.end(), [&](const std::string str) { transitions.push_back(std::make_pair(str, ñharacters)); });

	BuildingFirstRelationship(transitions, ñharacters);
	BuildingFirstPlusRelationship(transitions);

	for (auto& outputData : m_outputData)
	{
		size_t row = std::distance(transitions.begin(), GetIteratorFindIfVector(transitions, outputData.nonterminal));

		if (row < transitions.size())
		{
			std::vector<std::string> guideCharacters;
			std::string firstTerminal = outputData.terminals.front();

			for (size_t i = m_nonterminals.size(); i < transitions[row].second.size(); ++i)
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

void GuideSets::BuildingFirstRelationship(std::vector<PairStringVectorPair>& transitions, std::vector<PairStringBool>& ñharacters)
{
	for (auto& outputData : m_outputData)
	{
		size_t row = std::distance(transitions.begin(), GetIteratorFindIfVector(transitions, outputData.nonterminal));
		size_t column = std::distance(ñharacters.begin(), GetIteratorFindIfVector(ñharacters, outputData.terminals.front()));

		if (row < transitions.size() && column < ñharacters.size())
		{
			if (outputData.terminals.front() == NONTERMINAL_END_SEQUENCE)
			{
				SearchStartingTerminalsEmptyRules(outputData.nonterminal, outputData.nonterminal, transitions, ñharacters);
			}
			else
			{
				transitions[row].second[column].second = true;
			}
		}
	}
}

void GuideSets::SearchStartingTerminalsEmptyRules(const std::string parentNonterminal, const std::string nonterminal, std::vector<PairStringVectorPair>& transitions, std::vector<PairStringBool>& ñharacters)
{
	for (const auto& outputData : m_outputData)
	{
		auto it1 = std::find_if(outputData.terminals.begin(), outputData.terminals.end(), [&](const std::string& str) { return str == nonterminal; });

		if (it1 != outputData.terminals.end() && outputData.nonterminal != nonterminal)
		{
			size_t distance = std::distance(outputData.terminals.begin(), it1);
			size_t size = outputData.terminals.size() - 1;
			std::string terminal = distance <= size ? (distance < size ? outputData.terminals[distance + 1] : outputData.terminals.back()) : NONTERMINAL_END_SEQUENCE;

			size_t row = std::distance(transitions.begin(), GetIteratorFindIfVector(transitions, parentNonterminal));
			size_t column = std::distance(ñharacters.begin(), GetIteratorFindIfVector(ñharacters, terminal == TERMINAL_END_SEQUENCE ? NONTERMINAL_END_SEQUENCE : terminal));

			if (row < transitions.size() && column < ñharacters.size() && terminal != parentNonterminal)
			{
				transitions[row].second[column].second = true;
			}

			if (nonterminal == parentNonterminal)
			{
				for (auto& data : m_outputData)
				{
					auto it2 = std::find_if(data.terminals.begin(), data.terminals.end(), [&](const std::string& str) { return str == outputData.nonterminal; });

					if (it2 != data.terminals.end())
					{
						SearchStartingTerminalsEmptyRules(parentNonterminal, (*it2), transitions, ñharacters);
					}
				}
			}
		}
	}
}

void GuideSets::BuildingFirstPlusRelationship(std::vector<PairStringVectorPair>& transitions)
{
	for (auto& transition : transitions)
	{
		for (size_t j = 0; j < m_nonterminals.size(); ++j)
		{
			if (transition.second[j].second)
			{
				size_t row = std::distance(transitions.begin(), GetIteratorFindIfVector(transitions, transition.second[j].first));

				for (size_t k = 0; k < transitions[row].second.size(); ++k)
				{
					if (transitions[row].second[k].second)
					{
						transition.second[k].second = true;
					}
				}
			}
		}
	}
}

bool GuideSets::IsNonterminal(const std::string str) const
{
	return str.front() == '<' && str.back() == '>';
}

std::string GuideSets::GetRandomNonterminal() const
{
	std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (auto& str : m_nonterminals)
	{
		std::string substr = str.substr(1, str.length() - 2);
		size_t pos = characters.find(substr);
		if (pos != std::string::npos)
		{
			characters.erase(pos, substr.length());
		}
	}

	int random = std::rand() % characters.size();

	return std::string(1, characters[random]);
}

void GuideSets::PrintInfoVector(const std::vector<std::string>& vec) const
{
	if (!vec.empty())
	{
		std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<std::string>(m_output, " "));
		m_output << vec.back();
	}
}

void GuideSets::PrintResult()
{
	for (const auto& outputData : m_outputData)
	{
		m_output << outputData.nonterminal << SPACE << "-" << SPACE;
		PrintInfoVector(outputData.terminals);
		m_output << SPACE << "/" << SPACE;
		PrintInfoVector(outputData.guideCharacters);
		m_output << std::endl;
	}
}

std::vector<std::string> GuideSets::GetUniques(const std::vector<std::string>& collection) const
{
	std::vector<std::string> uniques;
	for (const auto& name : collection)
	{
		if (std::find(uniques.begin(), uniques.end(), name) == uniques.end())
		{
			uniques.push_back(name);
		}
	}

	return uniques;
}

std::vector<PairStringBool>::iterator GuideSets::GetIteratorFindIfVector(std::vector<PairStringBool>& vec, const std::string str) const
{
	return std::find_if(vec.begin(), vec.end(), [&](const PairStringBool& data) { return data.first == str; });
}

std::vector<PairStringVectorPair>::iterator GuideSets::GetIteratorFindIfVector(std::vector<PairStringVectorPair>& vec, const std::string str) const
{
	return std::find_if(vec.begin(), vec.end(), [&](const PairStringVectorPair& data) { return data.first == str; });
}