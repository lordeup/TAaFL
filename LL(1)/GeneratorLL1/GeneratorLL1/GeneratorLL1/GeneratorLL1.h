#pragma once
#include "Header.h"
#include "Utils.h"

std::vector<OutputData> GetGenerateData(const std::vector<OutputDataGuideSets>& inputDatas, const std::vector<std::string>& nonterminals);
void PrintResult(std::ostream& fileOutput, const std::vector<OutputData>& outputDatas);
