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

	GnucapController::~GnucapController() {}

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
}