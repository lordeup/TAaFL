#pragma once
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <sstream>
#include <string>
#include <vector>

// dot -Tpng -odia.png test.dot

using namespace boost;

const std::string INPUT_FILE_NAME = "input.txt";
const std::string OUTPUT_FILE_NAME = "output.txt";
const std::string OUTPUT_GRAPH_NAME = "outputGraph.dot";
const std::string AUTOMAT_MOORE = "moore";
const std::string AUTOMAT_MEALY = "mealy";

const std::string ERROR_FILE_NOT_EXIST = "This file does not exist";
const std::string ERROR_WRONG_DATA = "Wrong data\n";
const std::string ERROR_UNKNOWN_MACHINE = "Unknown machine\n";

using Edge = std::pair<int, int>;
using EdgeVector = std::vector<Edge>;
using IntVector = std::vector<int>;
using Graph = adjacency_list<vecS, vecS, directedS, property<vertex_color_t, default_color_type>, property<edge_weight_t, int>>;