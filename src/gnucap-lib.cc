#include "StdAfx.h"
#include "gnucap-lib.h"
#include "gnucap-lib\md.h"
#include "gnucap-lib\c_comand.h"
#include "gnucap-lib\u_opt.h"
#include "gnucap-lib\u_prblst.h"
#include "gnucap-lib\e_card.h"
#include "gnucap-lib\s__.h"

// DO NOT USE ANY IO METHODS INCLUDING THE COMMANDS < and <<

// Hooking into lower level functions in gnucap
CARD*	check_create_insert_parse(CS&,bool,CARD_LIST::fat_iterator&, CARD*);

int nodes[20];
void setProbeVoltage(unsigned int node, double voltage) {
	cout << "node " << node << ": " << voltage << "v" << endl;
}

namespace gnucap_lib {
	GnucapController::GnucapController() :
		lCount(0), cCount(0), rCount(0), vCount(0) {
		SET_RUN_MODE xx(rINTERACTIVE);
		CMD::cmdproc("print op v(nodes)");
	}

	GnucapController::~GnucapController() {
		for each (Component* c in components) 
			delete c;
	}

	void GnucapController::test() {
		insertComponent("Vsupply 1 0 5");
		insertComponent("R0 1 2 1k");
		insertComponent("R1 2 0 1k");
		CMD::cmdproc("print op v(nodes)");
		CMD::cmdproc("op");
	}

	
	void GnucapController::analyse() {
		// Search for a changed circuit component
		auto result = std::find_if(components.begin(), components.end(), 
			[](Component* c) {return c->changed;});

		if (!(result == components.end())) { 
			// Determine netlist nodes from Component::connections[]
			for each (Component* c in components) {
				c->changed = false; // clear indicator (because we are going to re-analyse)
				c->voltage = 0; // In case analysis fails
				for (int i = 0; i < c->leads; i++) c->nodes[i] = -1; // reset netlist nodes
			}
			// Find the first connected power supply, perhaps later we will be less arbitrary and focus only on relative voltages
			result = std::find_if(vSupplies.begin(), vSupplies.end(), 
				[](Component* c) {
					return c->connections[0].size() > 0 && c->connections[1].size() > 0;
			}); //This is not robust, power supply could be connected but not in loop, fix before using multiple supplies

			int nodeCount = 0;
			Component* mainSupply;
			if (result == vSupplies.end())  //No connected power supplies found
				return;  // voltages are alrady cleared so just go
			
			mainSupply = *result;
			mainSupply->nodes[0] = nodeCount;
			//mainSupply->setNodes(

			//Some sort of recursive node hunting technique goes here

			CMD::cmdproc("clear"); //Delete all components
			for each(Component* c in components)  // Add all connected components
				if (c->isActive())
					insertComponent(c->generateString());
		}
		CMD::cmdproc("op"); // TODO: replace with ac analysis (once hooks are tested)
	}


	void GnucapController::insertComponent(std::string command) {
		CARD_LIST* scope = &CARD_LIST::card_list;
		assert(scope);
		CARD_LIST::fat_iterator putbefore(scope, scope->end());
		CS cmd(command);
		CARD* brh = check_create_insert_parse(cmd,OPT::dupcheck,putbefore, NULL); // untested NULL
	}

	
    std::string GnucapController::dtos(double d) {
		std::stringstream conv;
		conv << d;
		return conv.str();
	}

	std::string GnucapController::makeName(char type, int id) {
		std::stringstream conv;
		conv << type << id;
		return conv.str();
	}
	
	Component* GnucapController::newResistor(double r){ 
		auto resistor = new Component(makeName('R', rCount++));
		resistor->value = dtos(r);
		components.push_back(resistor);
		return resistor;		
	}

	Component* GnucapController::newCapacitor(double c){ 
		auto cap = new Component(makeName('C', cCount++));
		cap->value = dtos(c);
		components.push_back(cap);
		return cap;
	}

	Component* GnucapController::newInductor(double l){ 
		auto inductor = new Component(makeName('L', lCount++));
		inductor->value = dtos(l);
		components.push_back(inductor);
		return inductor;
	}

	Component* GnucapController::newDCSupply(double v) {
		auto vSource = newSupply();
		vSource->value = dtos(v);
		components.push_back(vSource);
		return vSource;
	}
	
	Component* GnucapController::newACSupply(double v, double f, double b) {
		auto vSource = newSupply();
		std::stringstream conv;
		conv << "sin(offset=" << b << ", amplitude=" << v << ", frequency=" << f << ")";
		vSource->value = conv.str();
		components.push_back(vSource);
		return vSource;		
	}

	Component* GnucapController::newSupply() {
		vSupplies.push_back(new Component(makeName('V', vCount++)));
		return vSupplies.back();
	}

	Component::Component(string name)  : 
	_name(name)  {
		switch (name.c_str()[0]) {
		case 'M'://osfet
		case 'Q'://BJT
			leads = 3;
		default:
			leads = 2;
		}
		connections = new std::vector<Component*>[leads];
		nodes = new int[leads];
		for (int i = 0; i < leads; i++) nodes[i] = -1;
	}

	bool Component::isActive() {
		for (int i = 0; i < leads; i++) 
			if (nodes[i] == -1) return false;
		
		return true;
	}
	
	string Component::generateString() {
		std::stringstream conv;
		conv << _name << ' ';
		for (int i = 0; i < leads; i++) 
			conv << nodes[i] << ' ';
		conv << value;
		return conv.str();
	}

	bool Component::toggleConnection(Component *other, int lead, int otherLead) {
		changed = true;

		std::vector<Component*>::iterator result;
		result = std::find(connections[lead].begin(), connections[lead].end(), other);

		if (result == connections[lead].end()) { // if connections[lead] does not contain other
			connections[lead].push_back(other);
			other->connections[otherLead].push_back(this);
		} else {
			connections[lead].erase(result);
			result = std::find(other->connections[lead].begin(), connections[lead].end(), this);
			other->connections[otherLead].erase(result);
		}
	}
}