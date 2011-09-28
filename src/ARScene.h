#pragma once

#include "StdAfx.h"
#include "gnucap-lib.h"
#include "Wire.h"

#include "ARToolkit2.7OpenCV.h"
#include "MovieNode.h"
#include "gnucap-lib.h"

class ARScene;

struct ConnectionTimer {
	ARScene* component;
	int otherLead;
	DWORD startTime;
	bool active;
};

class ARScene :
	public osg::Group
{
public:
	ARScene(osg::Node* n, std::string markerPath, OPIRALibrary::Registration* r);
	ARScene(std::string modelPath, std::string markerPath, OPIRALibrary::Registration* r);
	ARScene(libconfig::Setting const &config, OPIRALibrary::RegistrationARToolkit* r);
	ARScene(libconfig::Setting const &config, OPIRALibrary::RegistrationARToolkit* r, Compenent* c);

	bool isMarkerVisible();
	void setMarkerVisible(bool visible);

	void setVisibility(bool visible);
	
	osg::Matrix const &getOriginMatrix();
	void setOriginMatrix(osg::Matrix const &transform);

	osg::Matrix const &getMarkerMatrix();
	void setMarkerMatrix(osg::Matrix const &transform);

	osg::Matrix const &getSceneMatrix();
	void setSceneMatrix(osg::Matrix const &transform);
	void setSceneMatrixToInitial();

	osg::Matrix const &getInitialSceneMatrix();

	osg::Node const &getSceneNode();

	string const &getMarkerID();

	void play();
	void play(vector<osg::Node*> nodesToPlay);

	void pause();
	void pause(vector<osg::Node*> nodesToPause);

	void goToAndPlay(double startTime);
	void goToAndPlay(double startTime, vector<osg::Node*> nodesToPlay);

	void setAnimationSpeed(double timeMultiplier);

	void updateTextures();

	/*virtual*/ int numLeads();
	void proximityCheck(ARScene* target, int lead);
	osg::Vec3d getLeadCoord(int lead);
	vector<Wire> wires;
	~ARScene(void);

protected:
	osg::ref_ptr<osg::Node> model;
	osg::ref_ptr<osg::MatrixTransform> originTransform;
	osg::ref_ptr<osg::MatrixTransform> sceneTransform;
	osg::Matrix markerMatrix;
	osg::Matrix initialSceneMatrix;
	osg::ref_ptr<osg::Switch> visible;
	bool markerVisible;
	string markerID;
	vector<osg::AnimationPathCallback*> animCallbacks;
	vector<osg::Node*> nodesWithAnimations;
//	vector<MovieNode*> nodesWithMovies;

	void init(osg::Node* n, std::string markerPath, OPIRALibrary::Registration* r);
	void findSpecialNodes(osg::Group* modelNodes);
	void findAnimCBNodes(osg::Group* modelNodes);
	osg::Node* findNamedNode(std::string const &searchName, osg::Node* currNode);

private:
	gnucap_lib::Component* component;
	ConnectionTimer* timers;
};


class ARSceneCallback :
	public osg::NodeCallback
{
public:
   virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
   {
      ARScene* nodeToUpdate = dynamic_cast<ARScene*> (node);
	  nodeToUpdate->updateTextures();
   }
};