#pragma once
#include "Header.h"

bool IsNonterminal(const std::string str);
std::string SubstrNonterminal(const std::string str);

void FillingData(std::ifstream& fileInput, std::vector<InputData>& inputDatas, std::vector<std::string>& nonterminals);
void Generate(const std::vector<InputData>& inputDatas, std::vector<OutputData>& outputDatas, const std::vector<std::string>& nonterminals);
void PrintResult(std::ostream& fileOutput, const std::vector<OutputData>& outputDatas);

void PrintInfoVector(std::ostream& fileOutput, const std::vector<std::string>& vec);
