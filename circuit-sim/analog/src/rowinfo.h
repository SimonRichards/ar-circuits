#pragma once
#include "stdafx.h"

namespace circuit_sim {
	class RowInfo {
	public:
		const static int ROW_NORMAL = 0;  // ordinary value
		const static int ROW_CONST  = 1;  // value is constant
		const static int ROW_EQUAL  = 2;  // value is equal to another value
		int nodeEq, type, mapCol, mapRow;
		double value;
		bool rsChanges; // row's right side changes
		bool lsChanges; // row's left side changes
		bool dropRow;   // row is not needed in matrix
		RowInfo() { type = ROW_NORMAL; }
		~RowInfo() {}
	};
}