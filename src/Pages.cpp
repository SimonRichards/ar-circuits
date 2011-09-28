#include "StdAfx.h"

#include "ARCircuits.h"


ARCircuit1_3::ARCircuit1_3(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r)
: ARCircuit(config, r), latch_dist(10), latched(0)
{
	activePole = NULL;
	if(scenes.size() >= 3){
		nail = scenes.at(0);
		northPole = scenes.at(1);
		southPole = scenes.at(2);
		if(scenes.size() > 3){
			cout << "Extra models defined for ARCircuit1_3 will be ignored\n";
		}
	}
	else{
		cerr << "Not enough models defined for ARCircuit1_3\n";
		return;
	}
	//scenes.at(0)->findNamedNode("
	//for each (ARScene* scene in scenes){
	//	string nodeName = scene->getSceneNode().getName();
	//	if(nodeName == "nail"){
	//		nail = scene;
	//	}
	//	else if(nodeName == "floatingNPole"){
	//		northPole = scene;
	//	}
	//	else if(nodeName == "floatingSPole"){
	//		southPole = scene;
	//	}
	//	else{
	//		cout << "Unexpected scene node: " << nodeName << endl;
	//	}
	//}
	if(!config.lookupValue("latch_dist", latch_dist)){
		latch_dist = 150;
		cout << "Latch distance not defined, set to " << latch_dist << endl;
	}
}

void ARCircuit1_3::updateARCircuit(){
	for each (ARScene* scene in scenes){
		scene->setVisibility(scene->isMarkerVisible());
		scene->setOriginMatrix(scene->getMarkerMatrix());
	}

	if(activePole == northPole){
		if(northPole->isMarkerVisible()){
			latched = false;
		}
	}
	else if(activePole == southPole){
		if(southPole->isMarkerVisible()){
			latched = false;
		}
	}
	else{
		if(northPole->isMarkerVisible())
			activePole = northPole;
		else if(southPole->isMarkerVisible())
			activePole = southPole;
		else{
			return;
		}
	}
	
	if(nail->isMarkerVisible()){
		osg::Vec3f nailPos = nail->getOriginMatrix().getTrans();
		osg::Vec3f polePos = activePole->getOriginMatrix().getTrans();
		osg::Vec3f nailToPole = polePos - nailPos;
		if(nailToPole.length() < latch_dist){
			nail->setOriginMatrix(activePole->getOriginMatrix());
			latched = true;
		}
	}
}