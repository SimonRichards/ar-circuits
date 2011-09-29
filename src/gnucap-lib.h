/**
* Wrapper functions written by Simon Richards - 24/09/2011
*/
#pragma once

namespace gnucap_lib {
	class Component;
	class GnucapWrapper;

	class GnucapWrapper {
	private:
		unsigned int lCount, cCount, rCount, vCount;
		string makeName(char type, int id);
		vector<Component*> components;
		static void insertComponent(std::string command);
		vector<Component*> vSupplies;
		string transCommand;
		Component* newSupply();

	public:
		GnucapWrapper(double fps, int steps);
		~GnucapWrapper();
		void analyse();
		void test();
		Component* newResistor(double r);
		Component* newCapacitor(double c);
		Component* newInductor(double l);
		Component* newDCSupply(double v);
		Component* newACSupply(double v, double f, double b = 0);
	};

	struct Connection {
		Component* other;
		int otherLead;
		Connection(Component* oc, int ol) :
		other(oc), otherLead(ol) {}
		~Connection(){}
	};

	class Component {
	private:
		bool changed;
		int nodes[2];
		string _value;
		string generateString();
		Component(string name);
		Component(string name, double value);
		void init();
        int setNodes(int lead, int nodeVal, int nodeCount);
		~Component();
		void calculateVoltage(std::map<int, double> &voltages);
        bool minimiseNodes();
		friend class GnucapWrapper;

	public:
		string _name;
		unsigned int leads;
		double voltage, current;
		vector<Connection> connections[2];
        vector<Connection>* connectionA;
        vector<Connection>* connectionB;
		bool toggleConnection(Component *other, int lead, int otherLead); // returns true if components are connected following action
		bool isActive();
	};
}
