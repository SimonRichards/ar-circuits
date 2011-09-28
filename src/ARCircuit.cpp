#include "StdAfx.h"
#include "Wire.h"
#include "ARCircuit.h"


ARCircuit::ARCircuit(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r)
{
	for(int i = 0; i < config["scenes"].getLength(); i++){
		scenes.push_back(new ARScene(config["scenes"][i], r));
	}
}

vector<ARScene*> ARCircuit::getScenes(){
	return scenes;
}

void ARCircuit::updateARCircuit(){
	for each (ARScene* scene in scenes){ //TODO: if markers are lost keep displaying their model in the last known location.
		scene->setVisibility(scene->isMarkerVisible());
		scene->updateTextures();
		scene->setOriginMatrix(scene->getMarkerMatrix());
	}

	// Check for node proximity
	unsigned int i, j;
	ARScene *subject, *target;
	for (i = 0; i < scenes.size(); i++) {
		subject = scenes[i];
		for (j = i+1; j < scenes.size(); j++) {
			target = scenes[j];
			if (subject != target) {
				for (int i = 0; i < subject->numLeads(); i++) {
					subject->proximityCheck(target, i);
				}
			}
		}
		for each (Wire w in subject->wires) {
			w.render();
		}
	}
}

ARCircuit::~ARCircuit(void)
{
}
