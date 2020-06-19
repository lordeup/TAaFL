#pragma once
#include "Header.h"
#include "Utils.h"

std::vector<OutputData> GetGenerateData(const std::vector<OutputDataGuideSets>& inputDatas);
void PrintResult(std::ostream& fileOutput, const std::vector<OutputData>& outputDatas);
