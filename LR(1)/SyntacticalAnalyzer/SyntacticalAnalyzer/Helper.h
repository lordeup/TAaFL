#pragma once
#include "LRData.h"
#include <string>
#include <sstream>
#include <stack>

std::string GetString(std::istringstream& iss);
int ParseNumber(const std::string str);
Symbol ParseSymbol(std::string str);
State ParseState(char fCh);
template <typename T>
bool isEmptyStack(std::stack<T>& stack, std::string stackName);