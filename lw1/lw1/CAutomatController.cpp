#include "CAutomatController.h"

CAutomatController::CAutomatController(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
	m_inputSize = 0;
	m_outputSize = 0;
	m_stateCount = 0;
	m_automat = Automat::UNKNOWN;
}

void CAutomatController::DataReading()
{
	std::string nameAutomat;

	m_input >> m_inputSize;
	m_input >> m_outputSize;
	m_input >> m_stateCount;
	m_input >> nameAutomat;

	try
	{
		SetAutomat(nameAutomat);
	}
	catch (const std::invalid_argument& error)
	{
		m_output << error.what();
		return;
	}

	if (m_automat == Automat::MOORE)
	{
		try
		{
			FillingDataMoore();
			TransferAutomatMealy();
		}
		catch (const std::invalid_argument& error)
		{
			m_output << error.what();
			return;
		}
	}
	else if (m_automat == Automat::MEALY)
	{
		FillingDataMealy();
		TransferAutomatMoore();
	}

	PrintInfo(m_edge);
	PrintChart();
}

void CAutomatController::SetAutomat(const std::string automat)
{
	if (automat == AUTOMAT_MOORE)
	{
		m_automat = Automat::MOORE;
	}
	else if (automat == AUTOMAT_MEALY)
	{
		m_automat = Automat::MEALY;
	}
	else
	{
		throw std::invalid_argument(ERROR_UNKNOWN_MACHINE);
	}
}

void CAutomatController::PrintInfo(const EdgeVector& edge) const
{
	size_t lineFeed = edge.size() / m_inputSize;
	for (size_t i = 0; i < edge.size(); ++i)
	{
		if (i % lineFeed == 0 && i != 0)
		{
			m_output << std::endl;
		}

		if (m_automat == Automat::MOORE)
		{
			m_output << edge[i].first << " " << edge[i].second << "\t";
		}
		else if (m_automat == Automat::MEALY)
		{
			m_output << edge[i].first << "\t";
		}
	}
}

void CAutomatController::PrintChart() const
{
	if (m_automat == Automat::MEALY)
	{
		IntVector weights(m_edge.size());
		size_t lineFeed = m_edge.size() / m_inputSize;
		int count = 1;
		for (size_t i = 0; i < m_edge.size(); ++i)
		{
			if (i % lineFeed == 0 && i != 0)
			{
				++count;
			}
			weights[i] = count;
		}

		Graph graph(m_edge.begin(), m_edge.end(), weights.begin(), m_stateCount);

		dynamic_properties dp;
		dp.property("weight", get(edge_weight, graph));
		dp.property("label", get(edge_weight, graph));
		dp.property("node_id", get(vertex_index, graph));
		std::ofstream ofs(OUTPUT_GRAPH_NAME);
		write_graphviz_dp(ofs, graph, dp);
	}

		if (m_automat == Automat::MOORE)
	{
		IntVector weights(m_edge.size());
		size_t lineFeed = m_edge.size() / m_inputSize;
		int count = 1;
		for (size_t i = 0; i < m_edge.size(); ++i)
		{
			if (i % lineFeed == 0 && i != 0)
			{
				++count;
			}
			weights[i] = count;
		}

		Graph graph(m_edge.begin(), m_edge.end(), weights.begin(), m_stateCount);

		dynamic_properties dp;
		dp.property("weight", get(edge_weight, graph));
		dp.property("label", get(edge_weight, graph));
		dp.property("node_id", get(vertex_index, graph));
		std::ofstream ofs(OUTPUT_GRAPH_NAME);
		write_graphviz_dp(ofs, graph, dp);
	}
}

void CAutomatController::FillingDataMoore()
{
	m_outputCharacter.resize(m_stateCount);

	for (int i = 0; i < m_stateCount; ++i)
	{
		m_input >> m_outputCharacter[i];
	}

	m_state.resize(m_inputSize);
	int number;

	for (size_t i = 0; i < m_state.size(); ++i)
	{
		m_state[i].resize(m_stateCount);
		for (int j = 0; j < m_stateCount; ++j)
		{
			m_input >> number;
			if (number >= m_stateCount)
			{
				throw std::invalid_argument(ERROR_WRONG_DATA);
			}
			m_state[i][j] = number;
		}
	}
}

void CAutomatController::TransferAutomatMealy()
{
	for (size_t i = 0; i < m_state.size(); ++i)
	{
		for (size_t j = 0; j < m_outputCharacter.size(); ++j)
		{
			m_edge.push_back({ m_state[i][j], m_outputCharacter[m_state[i][j]] });
		}
	}
}

void CAutomatController::FillingDataMealy()
{
	int size = m_inputSize * m_stateCount;
	m_mealyEdge.resize(size);

	for (size_t i = 0; i < m_mealyEdge.size(); ++i)
	{
		m_input >> m_mealyEdge[i].first >> m_mealyEdge[i].second;
	}
}

void CAutomatController::TransferAutomatMoore()
{
	EdgeVector copyMealyEdge(m_mealyEdge);

	std::copy(m_mealyEdge.begin(), m_mealyEdge.end(), copyMealyEdge.begin());
	std::sort(copyMealyEdge.begin(), copyMealyEdge.end());
	copyMealyEdge.erase(std::unique(copyMealyEdge.begin(), copyMealyEdge.end()), copyMealyEdge.end());

	m_edge.resize(m_inputSize * copyMealyEdge.size());

	for (size_t i = 0; i < copyMealyEdge.size(); ++i)
	{
		size_t indexEdge = i;
		int index = copyMealyEdge[i].first;

		for (int j = 0; j < m_inputSize; ++j)
		{
			auto it = std::find(copyMealyEdge.begin(), copyMealyEdge.end(), m_mealyEdge[index]);
			int indexFindCopy = int(std::distance(copyMealyEdge.begin(), it));
			m_edge[indexEdge] = { indexFindCopy, copyMealyEdge[i].second };

			if (j < m_inputSize - 1)
			{
				indexEdge += copyMealyEdge.size();
				index += m_stateCount;
			}
		}
	}
}