#pragma once
#include "stdafx.h"
#include "components.h"

namespace circuit_sim {

	class NodeLink {
	public:
		int num;
		Component* elm;
	};

	class Node {
	public:
		std::vector<NodeLink> links;
	};
}