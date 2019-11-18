#pragma once
#include <algorithm>
#include <fstream>
#include <map>
#include <regex>
#include <string>
#include <vector>

const std::string ERROR_FILE_NOT_EXIST = "This file does not exist";
const std::string ERROR_NOT_ENOUGH_ARGUMENTS = "The number of arguments does not match the task condition\n"
											   "Input should look: Determination.exe <input file> <output file>\n";
const std::string ERROR_WRONG_DATA = "Wrong data\n";

const std::string END = "#";
const std::string SYMBOL_I = "I";
const std::string SYMBOL_F = "F";

using VectorString = std::vector<std::string>;
using DualVectorString = std::vector<VectorString>;
using TripleVectorString = std::vector<DualVectorString>;

using PairStringDual = std::pair<std::string, DualVectorString>;
using VectorPairStringDual = std::vector<std::pair<std::string, DualVectorString>>;
using PairString = std::pair<std::string, std::string>;
using VectorPairString = std::vector<PairString>;
using DualVectorPairString = std::vector<VectorPairString>;
using TripleVectorPairString = std::vector<DualVectorPairString>;

using MapKeyString = std::map<std::string, size_t>;
using MapKeySize_t = std::map<size_t, std::string>;

using MapEdge = std::map<std::pair<size_t, std::string>, VectorPairString>;
