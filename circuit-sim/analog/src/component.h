#include "stdafx.h"

namespace analog {

	class Component {
	private:

	public:
		virtual void step() = 0;
		virtual void calculateCurrent() = 0;
		virtual void stamp() = 0;
	};

}