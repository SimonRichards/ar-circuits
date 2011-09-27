#pragma once

#include "StdAfx.h"

#include "Page.h"

class SelectablePage : public Page
{
public:
	SelectablePage(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r);
	virtual void updatePage();
private:
	ARScene* lastMissingScene;
};

class Page1_3 : public Page
{
public:
	Page1_3(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r);
	virtual void updatePage();
private:
	ARScene* nail;
	ARScene* northPole;
	ARScene* southPole;
	float latch_dist;
	bool latched;
	ARScene* activePole;
};

class SimpleCircuit : public Page
{
public:
	SimpleCircuit(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r);
	virtual void updatePage();
private:
	int connectionTimer;
	static const int CONNECTION_HOLD = 10;
};