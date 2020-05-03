#pragma once
#include <vector>
#include <string>

const std::string ERROR_FILE_NOT_EXIST = "This file does not exist";
const std::string ERROR_NOT_ENOUGH_ARGUMENTS = "The number of arguments does not match the task condition\n"
											   "Input should look: GeneratorLL1.exe <input file> <output file>\n";
const std::string TAB = "\t";
const std::string END_SEQUENCE = "#";

using VectorString = std::vector<std::string>;