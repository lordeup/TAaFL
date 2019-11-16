#pragma once
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <vector>

const std::string ERROR_FILE_NOT_EXIST = "This file does not exist";
const std::string ERROR_NOT_ENOUGH_ARGUMENTS = "The number of arguments does not match the task condition\n"
											   "Input should look: Determination.exe <input file> <output file>\n";
const std::string ERROR_WRONG_DATA = "Wrong data\n";

using VectorString = std::vector<std::string>;
using DualVectorString = std::vector<VectorString>;
using TripleVectorString = std::vector<DualVectorString>;

using VectorPairString = std::vector<std::pair<std::string, std::string>>;
using DualVectorPairString = std::vector<VectorPairString>;
using TripleVectorPairString = std::vector<DualVectorPairString>;

using MapEdge = std::map<std::pair<size_t, std::string>, VectorPairString>;
