#include "StdAfx.h"
#include "ARScene.h"
#include "osg.h"

using namespace OPIRALibrary;
using namespace std;


osg::Node* createLights(osg::Node* scene) {
    osg::Group* lightGroup = new osg::Group;
   
	osg::StateSet* stateset = scene->getOrCreateStateSet();

    // Create a local light.
	osg::Light* light = new osg::Light();
    light->setLightNum(0);
    light->setPosition(osg::Vec4(0.0f, 0.0f, 1.0, 0.0f));
    light->setAmbient(osg::Vec4(0.4f,0.4f,0.4f,1.0f));
    light->setDiffuse(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	light->setSpecular(osg::Vec4(0.4f,0.4f,0.4f,1.0f));
    light->setConstantAttenuation(0.2f);
    light->setLinearAttenuation(0.05f);
    light->setQuadraticAttenuation(0.05f);

	
    osg::LightSource* lightSource = new osg::LightSource;	
    lightSource->setLight(light);
	lightSource->setLocalStateSetModes(osg::StateAttribute::ON); 
    lightSource->setStateSetModes(*stateset, osg::StateAttribute::ON);
    lightGroup->addChild(lightSource);
	
    return lightGroup;
}


modelOSG::modelOSG(osg::Node *n) {
	model = n;
	transform = new osg::MatrixTransform(osg::Matrix::identity());
	osgTransform = new osg::MatrixTransform(osg::Matrix::identity());//osg::Matrix::rotate(osg::DegreesToRadians(180.0f), osg::X_AXIS));
	visible = new osg::Switch();
	visible->addChild(transform); transform->addChild(osgTransform); osgTransform->addChild(model); transform->addChild(createLights(this));
	this->addChild(visible);
};



osg::Node* findNamedNode(const std::string& searchName, osg::Node* currNode)
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