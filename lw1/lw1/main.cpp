#include <fstream>
#include "CAutomatController.h"


//void makeSimpleDot()
//{
//	using Edge = std::pair<int, int>;
//	using Graph = boost::adjacency_list< boost::vecS, boost::vecS, boost::directedS,
//		boost::property< boost::vertex_color_t, boost::default_color_type >,
//		boost::property< boost::edge_weight_t, double >
//	>;
//
//	const int VERTEX_COUNT = 15;
//	std::vector<Edge> edges = {
//		{0, 4}, {0, 6}, {0, 1}, {1, 6}, {1, 11},
//		{2, 6}, {2, 9}, {2, 11}, {3, 4}, {4, 5},
//		{5, 8}, {6, 7}, {7, 8}, {8, 13},
//		{9, 10}, {10, 13}, {11, 12}, {12, 13}, {13, 14},
//	};
//	std::vector<double> weights(edges.size());
//	std::fill(weights.begin(), weights.end(), 1.0);
//	weights[1] = 0.5;
//	weights[2] = 2.5;
//	weights[3] = 4.3;
//
//	Graph graph(edges.begin(), edges.end(), weights.begin(), VERTEX_COUNT);
//
//	boost::dynamic_properties dp;
//	dp.property("weight", boost::get(boost::edge_weight, graph));
//	dp.property("label", boost::get(boost::edge_weight, graph));
//	dp.property("node_id", boost::get(boost::vertex_index, graph));
//	std::ofstream ofs("test.dot");
//	boost::write_graphviz_dp(ofs, graph, dp);
//}


int main()
{
	std::ifstream fileInput(INPUT_FILE_NAME);

	if (!fileInput.is_open())
	{
		std::cerr << ERROR_FILE_NOT_EXIST << std::endl;
		return 1;
	}

	CAutomatController automatController(fileInput, std::cout);
	automatController.DataReading();

	return 0;
}
