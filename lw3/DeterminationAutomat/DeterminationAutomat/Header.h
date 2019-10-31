#pragma once
#include <fstream>
#include <regex>
#include <string>
#include <vector>

const std::string ERROR_FILE_NOT_EXIST = "This file does not exist";
const std::string ERROR_NOT_ENOUGH_ARGUMENTS = "The number of arguments does not match the task condition\n"
											   "Input should look: Determination.exe <input file> <output file>\n";
const std::string ERROR_WRONG_DATA = "Wrong data\n";

using VectorSize_t = std::vector<size_t>;
using VectorString = std::vector<std::string>;
using DualVectorSize_t = std::vector<VectorSize_t>;
using TripleVectorSize_t = std::vector<DualVectorSize_t>;