#pragma once

#include "StdAfx.h"

#include "ARCircuit.h"

class SelectableARCircuit : public ARCircuit
{
public:
	SelectableARCircuit(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r);
	virtual void updateARCircuit();
private:
	ARScene* lastMissingScene;
};

class ARCircuit1_3 : public ARCircuit
{
public:
	ARCircuit1_3(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r);
	virtual void updateARCircuit();
private:
	ARScene* nail;
	ARScene* northPole;
	ARScene* southPole;
	float latch_dist;
	bool latched;
	ARScene* activePole;
};

class SimpleCircuit : public ARCircuit
{
public:
	SimpleCircuit(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r);
	virtual void updateARCircuit();
private:
	int connectionTimer;
	static const int CONNECTION_HOLD = 10;
};