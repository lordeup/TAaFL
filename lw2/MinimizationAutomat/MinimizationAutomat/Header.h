#pragma once
#include <algorithm>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/graphviz.hpp>
//#include <boost/graph/iteration_macros.hpp>
#include <regex>
#include <string>
#include <vector>

//using namespace boost;

const std::string NUMBER_REGULAR_EXPRESSION = "[[:digit:]]+";

const std::string OUTPUT_FILE_NAME = "output.txt";
const std::string OUTPUT_GRAPH_NAME = "outputGraph.dot";
const std::string AUTOMAT_MOORE = "moore";
const std::string AUTOMAT_MEALY = "mealy";

const std::string SPASE = " ";
const std::string SYMBOL_Z = "z";
const std::string SYMBOL_Q = "q";
const std::string SYMBOL_X = "x";
const std::string SYMBOL_Y = "y";

const std::string LABEL = "label";
const std::string NODE_ID = "node_id";

const std::string ERROR_FILE_NOT_EXIST = "This file does not exist";
const std::string ERROR_NOT_ENOUGH_ARGUMENTS = "The number of arguments does not match the task condition\n"
											   "Input should look: TransformationAutomat.exe <input file>\n";
const std::string ERROR_WRONG_DATA = "Wrong data\n";
const std::string ERROR_UNKNOWN_MACHINE = "Unknown machine\n";

using Edge = std::pair<int, int>;
using VectorEdge = std::vector<Edge>;
using VectorInt = std::vector<int>;
using VectorString = std::vector<std::string>;
using DualVectorInt = std::vector<VectorInt>;
//using Graph = adjacency_list<vecS, vecS, directedS, property<vertex_color_t, default_color_type>, property<edge_weight_t, std::string>>;