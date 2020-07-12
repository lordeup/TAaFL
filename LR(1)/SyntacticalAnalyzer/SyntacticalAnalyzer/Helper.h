#pragma once
#include "LRData.h"
#include <sstream>
#include <stack>
#include <string>

std::string GetString(std::istringstream& iss);
int ParseNumber(const std::string str);
Symbol ParseSymbol(std::string str);
StateSymbol ParseState(char fCh);
bool IsNonterminal(const std::string str);