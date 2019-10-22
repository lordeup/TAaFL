#pragma once
#include "Header.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/iteration_macros.hpp>

using namespace boost;

using Graph = adjacency_list<vecS, vecS, directedS, property<vertex_color_t, default_color_type>, property<edge_weight_t, std::string>>;

class Visualization
{
public:
	Visualization(const VectorSize_t& outputMoore, const size_t size);
	~Visualization() = default;

	void GraphView() const;

private:
	size_t m_size;
	VectorSize_t m_outputMoore;
};