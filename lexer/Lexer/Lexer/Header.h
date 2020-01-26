#pragma once
#include <algorithm>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

const std::string ERROR_FILE_NOT_EXIST = "This file does not exist";
const std::string ERROR_NOT_ENOUGH_ARGUMENTS = "The number of arguments does not match the task condition\n"
											   "Input should look: Determination.exe <input file> <output file>\n";
const std::string ERROR_WRONG_DATA = "Wrong data\n";
const std::string SPACE = " ";
const char POINT = '.';
const char SLASH = '/';
const char STAR = '*';
const char PLUS = '+';
const char MINUS = '-';
const char APOSTROPHE = '\'';
const std::string QUOTATION = "\"";

using VectorString = std::vector<std::string>;
using VectorChar = std::vector<char>;