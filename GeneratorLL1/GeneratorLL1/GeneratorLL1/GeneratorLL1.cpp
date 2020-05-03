#include "GeneratorLL1.h"

GeneratorLL1::GeneratorLL1(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
}

void GeneratorLL1::Execute()
{
	FillingData();
	Generate();
	PrintResult();
}

void GeneratorLL1::FillingData()
{
	std::string line;
	while (std::getline(m_input, line))
	{
		std::istringstream iss(line);
		std::string str;

		InputData inputData;
		bool isRightSide = false;
		bool isGuideCharacter = false;

		while (iss >> str)
		{
			char firstCh = str.front();
			if (firstCh == '<' && str.back() == '>')
			{
				std::string substr = str.substr(1, str.length() - 2);
				if (isRightSide)
				{
					inputData.terminals.push_back(substr);
				}
				else
				{
					inputData.nonterminal = substr;
					m_nonterminals.push_back(substr);
				}
			}
			else if (firstCh == '/')
			{
				isGuideCharacter = true;
			}
			else if (isGuideCharacter)
			{
				inputData.guideCharacters.push_back(str);
			}
			else if (firstCh == '-')
			{
				isRightSide = true;
			}
			else
			{
				inputData.terminals.push_back(str);
			}
		}
		m_inputData.push_back(inputData);
	}
}

void GeneratorLL1::Generate()
{
	for (size_t i = 0; i < m_inputData.size(); ++i)
	{
		OutputData outputData;
		outputData.symbol = m_inputData[i].nonterminal;
		outputData.guideCharacters = m_inputData[i].guideCharacters;
		outputData.pointer = i == 0 ? m_inputData.size() + 1 : m_outputData.back().pointer + m_inputData[i - 1].terminals.size();

		m_outputData.push_back(outputData);
	}

	for (size_t i = 0; i < m_inputData.size(); ++i)
	{
		size_t terminalsSize = m_inputData[i].terminals.size();
		for (size_t j = 0; j < terminalsSize; ++j)
		{
			bool isNext = j + 1 < terminalsSize;

			OutputData outputData;
			outputData.symbol = m_inputData[i].terminals[j];
			outputData.guideCharacters = VectorString{ outputData.symbol };
			outputData.isShift = true;
			outputData.pointer = m_outputData.size() + 2;

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
				outputData.guideCharacters = VectorString{ END_SEQUENCE };
				outputData.pointer = 0;
				outputData.isShift = false;
			}
			else
			{
				auto it = std::find(m_nonterminals.begin(), m_nonterminals.end(), outputData.symbol);

				if (it != m_nonterminals.end())
				{
					VectorString characters;
					size_t distance = std::distance(m_nonterminals.begin(), it);
					size_t repeatCounter = std::count(m_nonterminals.begin(), m_nonterminals.end(), outputData.symbol);

					for (size_t k = 0; k < repeatCounter; ++k)
					{
						if (k < repeatCounter - 1)
						{
							m_outputData[distance + k].isError = false;
						}
						for (const auto& character : m_outputData[distance + k].guideCharacters)
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

			m_outputData.push_back(outputData);
		}
	}
}

void GeneratorLL1::PrintInfoVector(const VectorString& vec) const
{
	if (!vec.empty())
	{
		std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<std::string>(m_output, ", "));
		m_output << vec.back();
	}
}

void GeneratorLL1::PrintResult()
{
	m_output << "¹" << TAB << "Symbol" << TAB << "Characters" << TAB << "Shift" << TAB << "Error" << TAB << "Pointer" << TAB << "Stack" << TAB << "End" << std::endl;

	for (size_t i = 0; i < m_outputData.size(); ++i)
	{
		OutputData outputData = m_outputData[i];
		size_t counter = i + 1;

		m_output << counter << TAB << outputData.symbol << TAB;
		PrintInfoVector(outputData.guideCharacters);
		m_output << TAB << TAB << outputData.isShift << TAB << outputData.isError << TAB << outputData.pointer << TAB << outputData.isStack << TAB << outputData.isEnd << std::endl;
	}
}
