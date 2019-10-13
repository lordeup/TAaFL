#pragma once
#include "Header.h"
#include "IAutomat.h"

class Visualization
{
public:
	Visualization(const VectorInt& outputMoore, const int size, Automat automat);
	Visualization(const VectorEdge& outputMealy, const int size, Automat automat);
	~Visualization() = default;

	void GraphView() const;

private:
	VectorInt m_outputMoore;
	VectorEdge m_outputMealy;
	int m_size;
	Automat m_automat;
};