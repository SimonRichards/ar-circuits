#include "StdAfx.h"

#include "ARToolkit2.7OpenCV.h"
#include "Book.h"

string includeDir = "../resources/";
extern string markerDir;
extern string modelDir;

using namespace OPIRALibrary;
using namespace libconfig;

Book* book;

int main(int argc, char **argv) {
	//markerDir = "../resources/markers/";
	//modelDir = "../resources/models/";
	Config cfg;
	string configFile = "book.cfg";
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
		cerr << "Could not open config file: " << configFile << endl;
		return -1;
	} catch (const libconfig::ParseException &e) {
		cerr << "Error parsing file: " << e.getError() << " line " << e.getLine() << endl;
		return -1;
	}
	
	const Setting& root = cfg.getRoot();
	root.lookupValue("markerDir", markerDir);
	root.lookupValue("modelDir", modelDir);
	markerDir = includeDir + markerDir;
	modelDir = includeDir + modelDir;

	try{
		book = new Book(root["book"], argc, argv);
		book->run();
	}
	catch(libconfig::SettingNotFoundException e){
		cerr << "book not defined" << endl;
	}
}