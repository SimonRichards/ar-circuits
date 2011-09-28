#include "StdAfx.h"
#include "Wire.h"
#include "ARCircuit.h"


ARCircuit::ARCircuit(const libconfig::Setting& config, gnucap_lib::GnucapWrapper gnucap, OPIRALibrary::RegistrationARToolkit* r)
{
	double value = 100.;
	if(config.exists("resistors")){
		for(int i = 0; i < (int)config["resistors"]["number"]; i++){
			string markerFile = config["resistors"]["markerProto"];
			config.lookupValue("values[i]", value);
			scenes.push_back(new ARScene(config["resistors"]["model"], markerFile, gnucap.newResistor(value), r));
		}
	}
	if(config.exists("capacitors")){
		for(int i = 0; i < (int)config["capacitors"]["number"]; i++){
			string markerFile = config["capacitors"]["markerProto"];
			config.lookupValue("values[i]", value);
			scenes.push_back(new ARScene(config["capacitors"]["model"], markerFile, gnucap.newCapacitor(value), r));
		}
	}
	if(config.exists("inductors")){
		for(int i = 0; i < (int)config["inductors"]["number"]; i++){
			string markerFile = config["inductors"]["markerProto"];
			config.lookupValue("values[i]", value);
			scenes.push_back(new ARScene(config["inductors"]["model"], markerFile, gnucap.newInductor(value), r));
		}
	}
	if(config.exists("dcsupplies")){
		for(int i = 0; i < (int)config["dcsupplies"]["number"]; i++){
			string markerFile = config["dcsupplies"]["markerProto"];
			config.lookupValue("values[i]", value);
			scenes.push_back(new ARScene(config["dcsupplies"]["model"], markerFile, gnucap.newDCSupply(value), r));
		}
	}
	if(config.exists("acsupplies")){
		for(int i = 0; i < (int)config["acsupplies"]["number"]; i++){
			string markerFile = config["acsupplies"]["markerProto"];
			config.lookupValue("values[i]", value);
			scenes.push_back(new ARScene(config["acsupplies"]["model"], markerFile, gnucap.newACSupply(value, 50), r)); //TODO: 50Hz
		}
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

        for each (Wire* w in subject->wires)
            w->update();		
	}
}

ARCircuit::~ARCircuit(void)
{
}
