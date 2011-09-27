#pragma once

#include "StdAfx.h"

#include "ARScene.h"

class Page
{
public:
	Page(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r);
	Page(){}
	~Page(void);

	vector<ARScene*> getScenes();
	virtual void updatePage();

protected:
	vector<ARScene*> scenes;
};

