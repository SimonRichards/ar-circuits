#include "StdAfx.h"
#include "Wire.h"

#include "ARCircuit.h"


ARCircuit::ARCircuit(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r)
{
	initComponents(config["resistors"], r);
	initComponents(config["capacitors"], r);
	initComponents(config["inductors"], r);
	initComponents(config["acsupplies"], r);
}

void ARCircuit::initComponents(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r){
	for(int i = 0; i < config["markers"].getLength(); i++){
		scenes.push_back(new ARScene(config["markers"][i], r));
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

	for each (ARScene* subject in scenes) {
		for each (ARScene* target in scenes) {
			if (subject != target) {
				for (int i = 0; i < target->numLeads(); i++) {

				}
			}
		}
	}

	// Check proximity, adding/removing wires and togglingConnections()

	for each (Wire w in wires) {
		//w.setEndpoints();
		w.render();
	}
}

ARCircuit::~ARCircuit(void)
{
}
