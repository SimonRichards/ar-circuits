#pragma once

#include "StdAfx.h"
#include "Wire.h"
#include "ARScene.h"

class ARCircuit
{
public:
	ARCircuit(const libconfig::Setting& config, gnucap_lib::GnucapWrapper gnucap, OPIRALibrary::RegistrationARToolkit* r);
	ARCircuit(){}
	~ARCircuit(void);
	vector<ARScene*> getScenes();
	virtual void updateARCircuit();

protected:
	vector<ARScene*> scenes;
};

