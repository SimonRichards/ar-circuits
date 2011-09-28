#pragma once

#include "StdAfx.h"

#include "ARScene.h"

class ARCircuit
{
public:
	ARCircuit(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r);
	ARCircuit(){}
	~ARCircuit(void);

	vector<ARScene*> getScenes();
	virtual void updateARCircuit();

protected:
	vector<ARScene*> scenes;
};
