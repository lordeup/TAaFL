#include "GeneratorLR.h"

GeneratorLR::GeneratorLR(std::ofstream& fileOutput, const std::vector<OutputDataGuideSets>& inputDatas, const std::vector<std::string>& characters, const std::vector<TableData>& tableDataGuideSets)
	: m_output(fileOutput)
	, m_inputDatas(inputDatas)
	, m_characters(characters)
	, m_tableDataGuideSets(tableDataGuideSets)
{
}

void GeneratorLR::Generate()
{
	m_rightSides = GetRightSides();

	std::vector<Transition> transitions(m_characters.size());
	transitions.front().state = StateGenerator::START;

	std::vector<TableData> tableDatasFindGuideSets = GetTableDatasFindGuideSets(m_tableDataGuideSets, m_inputDatas.front().nonterminal);
	std::for_each(tableDatasFindGuideSets.begin(), tableDatasFindGuideSets.end(), [&](const TableData& data) { WriteTransitions(data, data.character, transitions); });

	AddingStackItems(transitions);

	m_outputGeneratorDatas.push_back({ TableData{ m_inputDatas.front().nonterminal, "Start" }, transitions });

	while (!m_stack.empty())
	{
		transitions.clear();
		transitions.resize(m_characters.size());

		std::vector<TableData> tableDatas = m_stack.top();
		m_stack.pop();

		std::for_each(tableDatas.begin(), tableDatas.end(), [&](const TableData& data) { FormingTransitions(data, transitions); });

		AddingStackItems(transitions);

		m_outputGeneratorDatas.push_back({ tableDatas.front(), transitions });
	}

	ConvertingTable();
}

void GeneratorLR::WriteTransitions(const TableData& tableData, const std::string str, std::vector<Transition>& transitions)
{
	AddShift(tableData, transitions);

	if (IsNonterminal(str))
	{
		std::vector<TableData> tableDatasFindGuideSets = GetTableDatasFindGuideSets(m_tableDataGuideSets, str);
		std::for_each(tableDatasFindGuideSets.begin(), tableDatasFindGuideSets.end(), [&](const TableData& data) { AddShift(data, transitions); });

		auto it = std::find_if(m_rightSides.begin(), m_rightSides.end(), [&](const TableData& data) { return data.nonterminal == str && IsEmptyRule(data.character); });

		if (it != m_rightSides.end())
		{
			FindRollUp(*it, (*it).nonterminal, (*it).nonterminal, transitions);
		}
	}
}

void GeneratorLR::FormingTransitions(const TableData& tableData, std::vector<Transition>& transitions)
{
	size_t nextIndex = tableData.position + 1;
	std::string nextStr = nextIndex < m_inputDatas[tableData.row].terminals.size() ? m_inputDatas[tableData.row].terminals[nextIndex] : "";

	if (!nextStr.empty() && !IsEndRule(nextStr))
	{
		TableData data{ tableData.nonterminal, nextStr, tableData.row, nextIndex };
		WriteTransitions(data, nextStr, transitions);
	}
	else
	{
		if (IsEndRule(nextStr))
		{
			AddRollUp(tableData, nextStr, transitions);
		}
		else
		{
			std::string str = m_inputDatas[tableData.row].terminals[tableData.position == 0 ? tableData.position : tableData.position - 1];

			if (IsNonterminal(str))
			{
				auto it = std::find_if(m_rightSides.begin(), m_rightSides.end(), [&](const TableData& data) { return data.nonterminal == str && IsEmptyRule(data.character); });

				if (it != m_rightSides.end())
				{
					std::vector<TableData> tableDatasFindGuideSets = GetTableDatasFindGuideSets(m_tableDataGuideSets, str);

					for (const auto& data : tableDatasFindGuideSets)
					{
						if (data.character != str)
						{
							AddRollUp(data, data.character, transitions);
						}
					}
				}
			}

			FindRollUp(tableData, m_inputDatas[tableData.row].nonterminal, m_inputDatas[tableData.row].nonterminal, transitions);
		}
	}
}

void GeneratorLR::AddingStackItems(std::vector<Transition>& transitions)
{
	for (const auto& transition : transitions)
	{
		if (transition.state == StateGenerator::SHIFT)
		{
			std::vector<TableData> vec;
			for (const auto& tableData : transition.tableDatas)
			{
				if (IsCheckTableDataUniqueness(m_states, tableData) && !IsEmptyRule(tableData.character))
				{
					vec.push_back(tableData);
					m_states.push_back(tableData);
				}
			}
			if (!vec.empty())
			{
				m_stack.push(vec);
			}
		}
	}
}

void GeneratorLR::AddShift(const TableData& tableData, std::vector<Transition>& transitions)
{
	AddEvent(tableData, tableData.character, StateGenerator::SHIFT, transitions);
}

void GeneratorLR::AddRollUp(const TableData& tableData, const std::string str, std::vector<Transition>& transitions)
{
	std::string newStr = IsEndRule(str) ? NONTERMINAL_END_SEQUENCE : str;
	AddEvent(tableData, newStr, StateGenerator::ROLL_UP, transitions);
}

void GeneratorLR::AddEvent(const TableData& tableData, const std::string str, const StateGenerator state, std::vector<Transition>& transitions)
{
	size_t row = GetDistanceVector(m_characters, str);

	if (row < m_characters.size())
	{
		if (IsCheckTableDataUniqueness(transitions[row].tableDatas, tableData))
		{
			transitions[row].state = state;
			transitions[row].tableDatas.push_back(tableData);
		}
	}
}

void GeneratorLR::FindRollUp(const TableData& tableData, const std::string parentNonterminal, const std::string nonterminal, std::vector<Transition>& transitions)
{
	for (auto it = m_rightSides.begin(); it != m_rightSides.end(); ++it)
	{
		it = std::find_if(it, m_rightSides.end(), [&](const TableData& data) { return data.character == nonterminal; });

		if (it == m_rightSides.end())
		{
			break;
		}

		size_t nextPosition = (*it).position + 1;

		if (nextPosition < m_inputDatas[(*it).row].terminals.size())
		{
			if (!IsNonterminal(m_inputDatas[(*it).row].terminals[nextPosition]))
			{
				AddRollUp(tableData, m_inputDatas[(*it).row].terminals[nextPosition], transitions);
			}
		}
		else if (parentNonterminal != (*it).nonterminal)
		{
			FindRollUp(tableData, parentNonterminal, m_inputDatas[(*it).row].nonterminal, transitions);
		}
	}
}

void GeneratorLR::ConvertingTable()
{
	for (const auto& outputGeneratorData : m_outputGeneratorDatas)
	{
		std::vector<std::string> transitions(outputGeneratorData.transitions.size());

		for (size_t j = 0; j < transitions.size(); ++j)
		{
			StateGenerator state = outputGeneratorData.transitions[j].state;
			if (state == StateGenerator::START)
			{
				transitions[j] = "Ok";
			}
			else if (state == StateGenerator::SHIFT)
			{
				for (const auto& tableData : outputGeneratorData.transitions[j].tableDatas)
				{
					size_t distance = std::distance(m_outputGeneratorDatas.begin(), std::find_if(m_outputGeneratorDatas.begin(), m_outputGeneratorDatas.end(), [&](const OutputDataGenerator& data) {
						return data.tableData == tableData;
					}));

					if (distance < m_outputGeneratorDatas.size())
					{
						transitions[j] = "S" + std::to_string(distance + 1);
						//break;
					}
				}
			}
			else if (state == StateGenerator::ROLL_UP)
			{
				transitions[j] = "R" + std::to_string(outputGeneratorData.transitions[j].tableDatas.back().row + 1);
			}
		}

		m_outputDatas.push_back({ outputGeneratorData.tableData, transitions });
	}
}

std::vector<TableData> GeneratorLR::GetTableDatasFindGuideSets(const std::vector<TableData>& vec, const std::string str)
{
	std::vector<TableData> tableDatasFindGuideSets;

	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		it = std::find_if(it, vec.end(), [&](const TableData& data) { return data.nonterminal == str; });
		if (it == vec.end())
		{
			break;
		}

		tableDatasFindGuideSets.push_back(*it);
	}

	return tableDatasFindGuideSets;
}

std::vector<TableData> GeneratorLR::GetRightSides()
{
	std::vector<TableData> vec;

	for (size_t i = 0; i < m_inputDatas.size(); ++i)
	{
		for (size_t j = 0; j < m_inputDatas[i].terminals.size(); ++j)
		{
			vec.push_back(TableData{ m_inputDatas[i].nonterminal, m_inputDatas[i].terminals[j], i, j });
		}
	}

	return vec;
}

void GeneratorLR::PrintResult()
{
	m_output << "Number" << TAB << "Char" << TAB;

	for (auto it = m_characters.begin(); it != m_characters.end(); ++it)
	{
		m_output << (IsEmptyRule(*it) ? TERMINAL_END_SEQUENCE : *it);

		if (it != m_characters.end() - 1)
		{
			m_output << TAB;
		}
	}

	m_output << std::endl;

	for (size_t i = 0; i < m_outputDatas.size(); ++i)
	{
		m_output << i + 1 << TAB << m_outputDatas[i].tableData.character << TAB;

		for (auto it = m_outputDatas[i].transitions.begin(); it != m_outputDatas[i].transitions.end(); ++it)
		{
			m_output << ((*it).empty() ? "-" : *it);

			if (it != m_outputDatas[i].transitions.end() - 1)
			{
				m_output << TAB;
			}
		}

		m_output << std::endl;
	}
}
