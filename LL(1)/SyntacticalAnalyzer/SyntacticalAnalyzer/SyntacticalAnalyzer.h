#pragma once
#include "Header.h"

void InitInputTable(std::ifstream& fileTableInput, std::vector<InputTableData>& inputTable);
void InitSentence(std::ifstream& fileSentenceInput, std::string& sentence);
void MakeProcess(std::vector<InputTableData>& inputTable, std::vector<OutputTableData>& outputTable, std::string& sentence);