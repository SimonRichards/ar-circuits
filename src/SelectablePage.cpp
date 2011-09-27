#include "StdAfx.h"

#include "Pages.h"


SelectablePage::SelectablePage(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r)
	: Page(config, r), lastMissingScene(scenes.at(0))
{
}

void SelectablePage::updatePage(){
	for each (ARScene* scene in scenes){
		if(scene->isMarkerVisible()){
			if(scene == lastMissingScene){
				scene->setVisibility(true);
			}
			else{
				scene->setVisibility(false);
			}
			scene->updateTextures();
			scene->setOriginMatrix(scene->getMarkerMatrix());
		}
		else{
			scene->setVisibility(false);
			lastMissingScene = scene;
		}
	}
}