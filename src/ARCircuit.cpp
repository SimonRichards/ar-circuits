#include "StdAfx.h"
#include "Wire.h"
#include "ARCircuit.h"
#include <iostream>


ARCircuit::ARCircuit(const libconfig::Setting& config, gnucap_lib::GnucapWrapper &gnucap, OPIRALibrary::RegistrationARToolkit* r)
{
    string types[] = {"resistors", "capacitors", "inductors", "dcsupplies", "acsupplies"};
    stringstream buffer;
    gnucap_lib::Component *c;

    for each (string type in types) {
        if(config.exists(type)) {
            int num = (int)config[type]["number"];
            for(int i = 0; i < num; i++){
                double value = 100;
                string markerFile = config[type]["markerProto"];
                buffer.clear();
                buffer << i;
                if (markerFile.find_first_of('X') == -1) {
                    cerr << "markerFile " << markerFile << " did not contain an X." << endl;
                    exit(-1);
                }
                markerFile.replace(markerFile.find_first_of('X'), 1, buffer.str());
                cout << markerFile << endl;
                buffer.clear();
                buffer << "values[" << i << ']';
                config.lookupValue(buffer.str(), value);
                switch (type.c_str()[0]) { //apologies to anyone who wishes to add a type with the same initial as a previous one
                case 'r': c = gnucap.newResistor(value); break;
                case 'c': c = gnucap.newCapacitor(value); break;
                case 'i': c = gnucap.newInductor(value); break;
                case 'd': c = gnucap.newDCSupply(value); break;
                case 'a': c = gnucap.newACSupply(value, 50); break;
                default: cerr << "invalid component type " << type <<endl;
                }
                scenes.push_back(new ARScene(config[type]["model"], markerFile, c, r));
            }
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
        if (!subject->isMarkerVisible()) continue;
        for (j = i+1; j < scenes.size(); j++) {
            target = scenes[j];
            if (!target->isMarkerVisible()) continue;
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
