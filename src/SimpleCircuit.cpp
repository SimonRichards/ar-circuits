#include "StdAfx.h"

#include "Pages.h"
//#include "CircuitComponent.h"

SimpleCircuit::SimpleCircuit(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r)
{
	for(int i = 0; i < config["scenes"].getLength(); i++){
		//scenes.push_back(new CircuitComponent(config["scenes"][i], r));
	}
}

void SimpleCircuit::updatePage(){
	//for each (CircuitComponent* subject in scenes){
	//	if(subject->isMarkerVisible()){
	//		subject->setVisibility(subject->isMarkerVisible());
	//		subject->setOriginMatrix(subject->getMarkerMatrix());
	//		for each (CircuitComponent* target in scenes){
	//			if(subject != target){
	//				subject->checkForConnect(target);
	//			}
	//		}
	//	}
	//}
}