/**
 * Wrapper functions for the Gnucap library
 */
#pragma once

namespace gnucap_lib {
    class GnucapController {
    private:
        
    public:
        GnucapController();
        ~GnucapController();
        void test();
    };

	class Component {
	private:
		char _type;
		bool active;
		friend class GnucapController;

	public:
		Component(char type) : active(false), _type(type){}
		~Component(){}
	};
}
