#include "StdAfx.h"
#include "gnucap-lib.h"
#include "gnucap-lib\md.h"
#include "gnucap-lib\c_comand.h"
#include "gnucap-lib\u_opt.h"
#include "gnucap-lib\u_prblst.h"
#include "gnucap-lib\e_card.h"
#include "gnucap-lib\s__.h"
//#define DEBUG
// DO NOT USE ANY IO METHODS INCLUDING THE COMMANDS < and <<

// Hooking into lower level functions in gnucap
CARD*	check_create_insert_parse(CS&,bool,CARD_LIST::fat_iterator&, CARD*);

std::map<int, double> nodeVoltages;
void setProbeVoltage(unsigned int node, double voltage) {
	nodeVoltages[node] = voltage;
#ifdef DEBUG
	cout << "node " << node << ": " << voltage << "v" << endl;
#endif
}

namespace gnucap_lib {
	
std::string dtos(double d) {
	std::stringstream conv;
	conv << d;
	return conv.str();
}


GnucapWrapper::GnucapWrapper(double fps, int steps) :
	lCount(0), cCount(0), rCount(0), vCount(0) {
	SET_RUN_MODE xx(rINTERACTIVE);
	stringstream conv;
	conv << "trans " << 1/fps << " " << (1/fps) / (double)steps;
	transCommand = conv.str();
}

GnucapWrapper::~GnucapWrapper() {
	for each (Component* c in components) 
		delete c;
}

void GnucapWrapper::test() {
	auto r1 = newResistor(100);
	auto r2 = newResistor(100);
	auto v = newDCSupply(12);
	auto c = newCapacitor(1e-9); // unused
	r1->toggleConnection(v, 0, 0);
	r2->toggleConnection(r1, 1, 1);
	r2->toggleConnection(v, 0, 1);
	
	analyse();
#ifdef DEBUG
	for each (Component* c in components) 
		cout << c->generateString() << " V = " << c->voltage << endl;
#endif
}


void GnucapWrapper::analyse() {
	unsigned int i;
	// Search for a changed circuit component
	auto result = std::find_if(components.begin(), components.end(), 
		[](Component* c) {return c->changed;});

	if (!(result == components.end())) { 
		// Determine netlist nodes from Component::connections[]
		for each (Component* c in components) {
			c->changed = false; // clear indicator (because we are going to re-analyse)
			c->voltage = 0; // In case analysis fails
			for (i = 0; i < c->leads; i++) c->nodes[i] = -1; // reset netlist nodes
		}
		// Find the first connected power supply, perhaps later we will be less arbitrary and focus only on relative voltages
		result = std::find_if(vSupplies.begin(), vSupplies.end(), 
			[](Component* c) {
				return c->connections[0].size() > 0 && c->connections[1].size() > 0;
		}); //This is not robust, power supply could be connected but not in loop, fix before using multiple supplies

		//int nodeCount = 0;
		Component* mainSupply;
		if (result == vSupplies.end())  //No connected power supplies found
			return;  // voltages are alrady cleared so just go

		mainSupply = *result;
		mainSupply->setNodes(0, 0, 0);

		if (mainSupply->nodes[1] == -1) {
#ifdef DEBUG
			cout << "Circuit incomplete" << endl;
#endif
			return;
		}

		CMD::cmdproc("clear"); //Delete all components
		for each(Component* c in components)  // Add all connected components
			if (c->isActive())
				insertComponent(c->generateString());
		CMD::cmdproc("print trans v(nodes)");
		CMD::cmdproc(transCommand); 
	}
	
	CMD::cmdproc("trans"); 
	for each(Component* c in components) {
		c->calculateVoltage(nodeVoltages);
	}
}


void GnucapWrapper::insertComponent(std::string command) {
	CARD_LIST* scope = &CARD_LIST::card_list;
	assert(scope);
	CARD_LIST::fat_iterator putbefore(scope, scope->end());
	CS cmd(command);
	CARD* brh = check_create_insert_parse(cmd,OPT::dupcheck,putbefore, NULL); // untested NULL
}



std::string GnucapWrapper::makeName(char type, int id) {
	std::stringstream conv;
	conv << type << id;
	return conv.str();
}

Component* GnucapWrapper::newResistor(double r){ 
	auto resistor = new Component(makeName('R', rCount++), r);
	components.push_back(resistor);
	return resistor;		
}

Component* GnucapWrapper::newCapacitor(double c){ 
	auto cap = new Component(makeName('C', cCount++), c);
	components.push_back(cap);
	return cap;
}

Component* GnucapWrapper::newInductor(double l){ 
	auto inductor = new Component(makeName('L', lCount++), l);
	components.push_back(inductor);
	return inductor;
}

Component* GnucapWrapper::newDCSupply(double v) {
	auto vSource = newSupply();
	vSource->_value = dtos(v);
	components.push_back(vSource);
	return vSource;
}

Component* GnucapWrapper::newACSupply(double v, double f, double b) {
	auto vSource = newSupply();
	std::stringstream conv;
	conv << "sin(offset=" << b << ", amplitude=" << v << ", frequency=" << f << ")";
	vSource->_value = conv.str();
	components.push_back(vSource);
	return vSource;		
}

Component* GnucapWrapper::newSupply() {
	vSupplies.push_back(new Component(makeName('V', vCount++)));
	return vSupplies.back();
}

void Component::calculateVoltage(std::map<int, double> &voltages) {
	voltage = voltages[nodes[0]] - voltages[nodes[1]]; //TODO: calculate more voltages for trannies and other components where leads!=2
}

int Component::setNodes(int lead, int nodeVal, int nodeCount) {
	unsigned int i, j;
	if (nodes[lead] != -1)
		return nodeCount;
	nodes[lead] = nodeCount;
	for (i = 0; i < connections[lead].size(); i++) {
		nodeCount = connections[lead][i].other->setNodes(connections[lead][i].otherLead, nodeVal, nodeCount);
	}
	for (j = 0; j < leads; j++) {
		if (j != lead) {
			nodeCount++;
			for (i = 0; i < connections[j].size(); i++) {
				nodeCount = connections[j][i].other->setNodes(connections[j][i].otherLead, nodeVal, nodeCount);
			}
		}
	}

	return nodeCount;
}

Component::Component(string name, double value)  : 
_name(name)  {
	init();
	_value = dtos(value);
}

Component::Component(string name)  : 
_name(name)  {
	init();
}

void Component::init() {
    cout << "new component" <<endl;
	switch (_name.c_str()[0]) {
	case 'M'://osfet
	case 'Q'://BJT
		leads = 3;
	default:
		leads = 2;
	}
	connections = new std::vector<Connection>[leads];
	nodes = new int[leads];
	for (unsigned int i = 0; i < leads; i++) nodes[i] = -1;
}

Component::~Component() {
//	delete connections; TODO, FIXME: destructor fails
	delete nodes;
}

bool Component::isActive() {
	for (unsigned int i = 0; i < leads; i++) 
		if (nodes[i] == -1) return false;

	return true;
}

string Component::generateString() {
	std::stringstream conv;
	conv << _name << ' ';
	for (unsigned int i = 0; i < leads; i++) 
		conv << nodes[i] << ' ';
	conv << _value;
	return conv.str();
}

bool Component::toggleConnection(Component *other, int lead, int otherLead) {
	other->changed = changed = true;

	auto result = std::find_if(connections[lead].begin(), connections[lead].end(), 
		[](Connection connection) {return connection.other == 0;});

	if (result == connections[lead].end()) { // if connections[lead] does not contain other
		connections[lead].push_back(Connection(other, otherLead));
		other->connections[otherLead].push_back(Connection(this, lead));
		return true;
	} else { // if it does
		connections[lead].erase(result);
		unsigned int i;
		for (i = 0; i < other->connections[otherLead].size(); i++) {
			if (other->connections[otherLead][i].other == this) {
				other->connections[otherLead].erase(other->connections[otherLead].begin() + i);
				break;
			}
		}

		if (i == other->connections[otherLead].size()) cerr << "connection toggle error" << endl;
		return false;
	}
}

}
