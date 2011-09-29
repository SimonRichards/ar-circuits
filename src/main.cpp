#include "StdAfx.h"

//#include "vld.h"
#include "ARToolkit2.7OpenCV.h"
#include "Book.h"
#include <direct.h>


string includeDir = "resources/";
string markerDir = "markers/";
string modelDir = "models/";

unsigned long proximityDelay = 5000;
int proximityThreshold = 800;

using namespace OPIRALibrary;
using namespace libconfig;


int main(int argc, char **argv) {
	Config cfg;
	string configFile = "electro.cfg";
	for(int i = 0; i < argc; i++){
		string arg = argv[i];
		if(arg == "-cfg"){
			configFile = argv[i+1];
		cout << "arg: " << argv[i] << endl;
		}
		else if(arg == "-cfgDir"){
			includeDir = argv[i+1];
		}
	}
	cfg.setIncludeDir(includeDir.c_str());
	try {
		configFile = includeDir + configFile;
		cfg.readFile(configFile.c_str());
	} catch (const libconfig::FileIOException &e) {
		char buff[50];
		_getcwd(buff, 50);
		cout << buff;
		cerr << "Could not open config file: " << configFile << endl;
		return -1;
	} catch (const libconfig::ParseException &e) {
		cerr << "Error parsing file: " << e.getError() << " line " << e.getLine() << endl;
		return -1;
	}
	
	const Setting& root = cfg.getRoot();
	root.lookupValue("markerDir", markerDir);
	root.lookupValue("modelDir", modelDir);
	root.lookupValue("proximityThreshold", proximityThreshold);
	int temp = proximityDelay;
	root.lookupValue("proximityDelay", temp);
	proximityDelay = temp;
	markerDir = includeDir + markerDir;
	modelDir = includeDir + modelDir;

	try{
		Book book(root["book"], argc, argv);
		book.run();
	}
	catch(libconfig::SettingNotFoundException e){
		cerr << "book not defined" << endl;
        return -1;
	}
    
    _CrtDumpMemoryLeaks();
    cout << "Game Over" << endl;
    return 0;
}