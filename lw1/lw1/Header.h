#pragma once
#include <iostream>
//#include <boost/graph/graphviz.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/iteration_macros.hpp>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

// dot -Tpng -odia.png test.dot


const std::string INPUT_FILE_NAME = "input.txt";
const std::string AUTOMAT_MOORE = "moore";
const std::string AUTOMAT_MEALY = "mealy";


const std::string ERROR_FILE_NOT_EXIST = "This file does not exist";

//using Edge = std::pair<int, int>;

using Edge = std::vector<int>;