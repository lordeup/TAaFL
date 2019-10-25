#pragma once

class IAutomat
{
public:
	IAutomat() = default;
	virtual ~IAutomat() = default;

	virtual void GraphView() const = 0;
	virtual void Determination() = 0;
	virtual void PrintInfo() = 0;
};