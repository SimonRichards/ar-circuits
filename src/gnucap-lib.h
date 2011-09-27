/**
* Wrapper functions written by Simon Richards - 24/09/2011
*/
#pragma once

namespace gnucap_lib {
	class Component;
	class GnucapController;

	class GnucapController {
	private:
		unsigned int lCount, cCount, rCount, vCount;
		string dtos(double d);
		string makeName(char type, int id);
		vector<Component*> components;
		static void insertComponent(std::string command);
		vector<Component*> vSupplies;

	public:
		GnucapController();
		~GnucapController();
		void analyse();
		void test();
		Component* newResistor(double r);
		Component* newCapacitor(double c);
		Component* newInductor(double l);
		Component* newDCSupply(double v);
		Component* newACSupply(double v, double f, double b = 0);
		Component* newSupply();

	};
	class Component {
		string _name;
		bool changed;
		int *nodes;
		string value;
		string generateString();
		Component(string name);
		~Component();
		friend class GnucapController;

	public:

		int leads;
		double voltage;
		vector<Component*>* connections;
        void setNodes(int lead, int nodeCount);
		bool toggleConnection(Component *other, int lead, int otherLead); // returns true if components are connected following action
		bool isActive();
	};
}
