#pragma once
#include <iostream>
//#include <boost/graph/graphviz.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/iteration_macros.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

// dot -Tpng -odia.png test.dot

const std::string INPUT_FILE_NAME = "input4.txt";
const std::string OUTPUT_FILE_NAME = "output.txt";
const std::string AUTOMAT_MOORE = "moore";
const std::string AUTOMAT_MEALY = "mealy";

const std::string ERROR_FILE_NOT_EXIST = "This file does not exist";
const std::string ERROR_WRONG_DATA = "Wrong data\n";

using Edge = std::pair<int, int>;
using EdgeVector = std::vector<Edge>;