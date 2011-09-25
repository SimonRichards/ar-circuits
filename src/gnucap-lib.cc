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
	GnucapController::GnucapController() {
		SET_RUN_MODE xx(rINTERACTIVE);
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

	void GnucapController::runProbes() {
	}

	void GnucapController::insertComponent(std::string command) {
		CARD_LIST* scope = &CARD_LIST::card_list;
		assert(scope);

		CARD_LIST::fat_iterator putbefore(scope, scope->end());

		CS cmd(command);

		// check for dups, create, insert, parse
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
		static unsigned int rCount = 0;
		auto resistor = new Component(makeName('R', rCount++));
		resistor->value = dtos(r);
		components.push_back(resistor);
		return resistor;		
	}

	Component* GnucapController::newCapacitor(double c){ 
		static unsigned int cCount = 0;
		auto cap = new Component(makeName('C', cCount++));
		cap->value = dtos(c);
		components.push_back(cap);
		return cap;
	}

	Component* GnucapController::newInductor(double l){ 
		static unsigned int lCount = 0;
		auto inductor = new Component(makeName('L', lCount++));
		inductor->value = dtos(l);
		components.push_back(inductor);
		return inductor;
	}

	Component* GnucapController::newDCSupply(double v){ }
	Component* GnucapController::newACSupply(double v, double f) { 
		return newACSupply(v, 0, f, 0);
	} 

	Component* GnucapController::newACSupply(double v, double b, double f, double p) {
		
	}

	Component::Component(string name)  : 
	active(false), _name(name)  {
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
}