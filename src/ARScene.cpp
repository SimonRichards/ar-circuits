#include "StdAfx.h"

#include "ARScene.h"

extern string markerDir;
extern string modelDir;
extern unsigned long proximityDelay;
extern double proximityThreshold;
ARScene::ARScene(const libconfig::Setting& modelCfg, string markerFile, gnucap_lib::Component* c, OPIRALibrary::RegistrationARToolkit* r)
	: markerVisible(false), markerMatrix(osg::Matrix::identity()), initialSceneMatrix(osg::Matrix::identity())
{
	component = c;

	string modelFile = modelCfg["file"].c_str();

	timers = new ConnectionTimer[component->leads];

	model = osgDB::readNodeFile(modelDir + modelFile);
	if(modelCfg.exists("translation") && modelCfg["translation"].getLength() == 3)
		initialSceneMatrix.preMultTranslate(
		osg::Vec3d(modelCfg["translation"][0], modelCfg["translation"][1], modelCfg["translation"][2]));
	if(modelCfg.exists("rotation") && modelCfg["rotation"].getLength() == 3)
		initialSceneMatrix.preMultRotate(
		osg::Quat(osg::DegreesToRadians((float)modelCfg["rotation"][0]), osg::X_AXIS,
		osg::DegreesToRadians((float)modelCfg["rotation"][1]), osg::Y_AXIS,
		osg::DegreesToRadians((float)modelCfg["rotation"][2]), osg::Z_AXIS));
	if(modelCfg.exists("scale") && modelCfg["scale"].getLength() == 3)
		initialSceneMatrix.preMultScale(
		osg::Vec3d(modelCfg["scale"][0], modelCfg["scale"][1], modelCfg["scale"][2]));
	initialSceneMatrix.preMultRotate(osg::Quat(osg::PI, osg::X_AXIS));
	sceneTransform = new osg::MatrixTransform(initialSceneMatrix);
	originTransform = new osg::MatrixTransform();

	visible = new osg::Switch();
	visible->addChild(originTransform); originTransform->addChild(sceneTransform); sceneTransform->addChild(model); //transform->addChild(createLights(this));
	this->addChild(visible);
	markerID = markerDir + markerFile;
	r->addMarker(markerID, 80, 80);

	osg::Group* modelNodes = model->asGroup();

	for(int i = 0; i < modelCfg["animNodes"].getLength(); i++){
		nodesWithAnimations.push_back(findNamedNode(modelCfg["animNodes"][i]["name"], model));
	}/*
	 for(int i = 0; i < modelCfg["movieNodes"].getLength(); i++){
	 vector<string> imgPaths;
	 for(int j = 0; j < modelCfg["movieNodes"][i]["texturePaths"].getLength(); j++){
	 imgPaths.push_back(modelCfg["movieNodes"][i]["texturePaths"][j]);
	 }
	 bool autoPlay = true;
	 modelCfg["movieNodes"][i].lookupValue("autoPlay", autoPlay);
	 nodesWithMovies.push_back(new MovieNode(findNamedNode(modelCfg["movieNodes"][i]["name"], model), imgPaths, autoPlay));
	 }*/
	//setUpdateCallback(new ARSceneCallback);
}

osg::Matrix const &ARScene::getOriginMatrix(){
	return originTransform->getMatrix();
}
void ARScene::setOriginMatrix(osg::Matrix const &transform){
	originTransform->setMatrix(transform);
}

osg::Matrix const &ARScene::getMarkerMatrix(){
	return markerMatrix;
}
void ARScene::setMarkerMatrix(osg::Matrix const &transform){
	markerMatrix = transform;
}

osg::Matrix const &ARScene::getSceneMatrix(){
	return sceneTransform->getMatrix();
}
void ARScene::setSceneMatrix(osg::Matrix const &transform){
	sceneTransform->setMatrix(transform);
}
void ARScene::setSceneMatrixToInitial(){
	sceneTransform->setMatrix(initialSceneMatrix);
}

osg::Matrix const &ARScene::getInitialSceneMatrix(){
	return initialSceneMatrix;
}

bool ARScene::isMarkerVisible(){
	return markerVisible;
}

void ARScene::setMarkerVisible(bool visible){
	markerVisible = visible;
}

void ARScene::setVisibility(bool vis){
	if(vis){
		visible->setAllChildrenOn();
	}
	else{
		visible->setAllChildrenOff();
	}
}

int ARScene::numLeads() {
	return component->leads;
}

void ARScene::play(){
	play(vector<Node*>());
	/*
	for each(MovieNode* node in nodesWithMovies){
	node->nextFrame(); //TODO [test]
	}*/
}
void ARScene::play(vector<osg::Node*> nodesToPlay){
	if(nodesToPlay.size() == 0){
		nodesToPlay = nodesWithAnimations;
	}
	for each(osg::Node* node in nodesToPlay){
		osg::AnimationPathCallback* animCallback = dynamic_cast<osg::AnimationPathCallback*>(node->getUpdateCallback());
		if(animCallback){
			animCallback->setPause(false);
		}
	}

}

void ARScene::pause(){
	pause(vector<Node*>());
}
void ARScene::pause(vector<osg::Node*> nodesToPause){
	if(nodesToPause.size() == 0){
		nodesToPause = nodesWithAnimations;
	}
	for each(osg::Node* node in nodesToPause){
		osg::AnimationPathCallback* animCallback = dynamic_cast<osg::AnimationPathCallback*>(node->getUpdateCallback());
		if(animCallback){
			animCallback->setPause(false);
		}
	}

}

void ARScene::goToAndPlay(double startTime){
	goToAndPlay(startTime, vector<Node*>());
}
void ARScene::goToAndPlay(double startTime, vector<osg::Node*> nodesToPlay){
	if(nodesToPlay.size() == 0){
		nodesToPlay = nodesWithAnimations;
	}
	for each(osg::Node* node in nodesToPlay){
		osg::AnimationPathCallback* animCallback = dynamic_cast<osg::AnimationPathCallback*>(node->getUpdateCallback());
		if(animCallback){
			animCallback->setTimeOffset(0);
			//animCallback->reset();
			animCallback->setPause(false);
		}
	}
}

string const &ARScene::getMarkerID(){
	return markerID;
}

osg::Node const &ARScene::getSceneNode(){
	return *model;
}

osg::Node* ARScene::findNamedNode(const std::string& searchName, osg::Node* currNode)
{
	osg::Group* currGroup;
	osg::Node* foundNode;

	// check to see if we have a valid (non-NULL) node.
	// if we do have a null node, return NULL.
	if ( !currNode)
	{
		return NULL;
	}

	// We have a valid node, check to see if this is the node we 
	// are looking for. If so, return the current node.
	if (currNode->getName() == searchName)
	{
		return currNode;
	}

	// We have a valid node, but not the one we are looking for.
	// Check to see if it has children (non-leaf node). If the node
	// has children, check each of the child nodes by recursive call.
	// If one of the recursive calls returns a non-null value we have
	// found the correct node, so return this node.
	// If we check all of the children and have not found the node,
	// return NULL
	currGroup = currNode->asGroup(); // returns NULL if not a group.
	if ( currGroup ) 
	{
		for (unsigned int i = 0 ; i < currGroup->getNumChildren(); i ++)
		{ 
			foundNode = findNamedNode(searchName, currGroup->getChild(i));
			if (foundNode)
				return foundNode; // found a match!
		}
		return NULL; // We have checked each child node - no match found.
	}
	else 
	{
		return NULL; // leaf node, no match 
	}
}

void ARScene::updateTextures(){
	//cout << "up\n";
	/*
	for each (MovieNode* movie in nodesWithMovies){
	if(movie->isPlaying()){
	movie->nextFrame();
	}
	}*/
	goToAndPlay(0.5);
}

ARScene::~ARScene(void)
{
	delete timers;
}

osg::Vec3d ARScene::getCoord(int lead) {
	//TODO
	osg::Vec3d result;
	return result;
}


/**
 *@param target the ARScene to check against
 *@param lead the local lead to check
 */
void ARScene::proximityCheck(ARScene* target, int lead) {
	for (int i = 0; i < target->numLeads(); i++) {
		if (timers[lead].active && timers[lead].component == target && timers[lead].otherLead == i) {
			if ((getCoord(lead) - target->getCoord(i)).length2() < proximityThreshold) {
				if (GetTickCount() - timers[lead].startTime > proximityDelay) {
					if (component->toggleConnection(target->component, lead, i)) {
                        auto w = new Wire(this, target, lead, i);
						this->addChild(w);
                        wires.push_back(w);
					} else {
                        for (unsigned int j = 0; j < wires.size(); j++)  {
                            if (wires[j]->is(this, target, lead, i)) {
                                removeChild(wires[j]); //may not work
                                wires.erase(wires.begin()+j);
								break;
							}
						}
					} 
				}
			} else {
				timers[lead].active = false;
			}
		} else if (!timers[lead].active && ((getCoord(lead) - target->getCoord(i)).length2() < proximityThreshold)) {
			timers[lead].active = true;
			timers[lead].component = target;
			timers[lead].otherLead = i;
			timers[lead].startTime = GetTickCount();
		}
	}
}
