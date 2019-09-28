#pragma once
#include "Header.h"

class IAutomat
{
public:
	IAutomat() = default;
	virtual ~IAutomat() = default;

	virtual void GraphView() = 0;
	virtual void TransferAutomat() = 0;
	virtual void PrintInfo() = 0;
};

enum class Automat
{
	UNKNOWN,
	MOORE,
	MEALY
};