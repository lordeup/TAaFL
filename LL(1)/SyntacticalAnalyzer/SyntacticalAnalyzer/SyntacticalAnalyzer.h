#pragma once
#include "Header.h"

void InitInputTable(std::ifstream& fileTableInput, std::vector<InputTableData>& inputTable);
void InitSentence(std::ifstream& fileSentenceInput, std::string sentence);