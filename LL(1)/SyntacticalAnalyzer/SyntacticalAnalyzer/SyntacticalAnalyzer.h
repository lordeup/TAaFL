#pragma once
#include "Header.h"
#include "Lexer.h"

void InitInputTable(std::ifstream& fileTableInput, std::vector<InputTableData>& inputTable);
void MakeProcess(std::vector<InputTableData>& inputTable, std::vector<OutputTableData>& outputTable, const std::vector<Token>& tokens);
void PrintResult(std::ofstream& fileOutput, const std::vector<OutputTableData>& outputTable);