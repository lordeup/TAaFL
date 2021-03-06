#pragma once

class IAutomat
{
public:
	IAutomat() = default;
	virtual ~IAutomat() = default;

	virtual void GraphView() const = 0;
	virtual void TransferAutomat() = 0;
	virtual void PrintInfo() const = 0;
};