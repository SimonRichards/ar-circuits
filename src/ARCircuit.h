#pragma once

#include "StdAfx.h"
#include "Wire.h"
#include "ARScene.h"

class ARCircuit
{
public:
	ARCircuit(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r);
	ARCircuit(){}
	~ARCircuit(void);
	vector<Wire> wires;
	vector<ARScene*> getScenes();
	virtual void updateARCircuit();

protected:
	vector<ARScene*> scenes;

private:
	void initComponents(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r);
};

