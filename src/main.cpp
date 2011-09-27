#include "StdAfx.h"
#include "RegistrationARToolkit.h"
#include "ARScene.h"
#include "osg.h"
#include "gnucap-lib.h"
//#define GNUCAP_TESTING
using namespace OPIRALibrary;
using namespace std;

#define FPS 20
#define STEPS 10

osg::ref_ptr<osg::Camera> fgCamera;
vector<ARScene *> scenes;

vector<string> markerPaths(0);
osg::ref_ptr<modelOSG> batMask; 
osg::ref_ptr<modelOSG> theatreMask;
osgViewer::Viewer viewer;
osg::ref_ptr<osg::Image> mVideoImage;

float bX=44, bY=142, bZ=113, bS=310;
float tX=17, tY=232, tZ=17, tS=8;

inline void theatreTrans(osg::ref_ptr<osg::MatrixTransform> trans) { trans->setMatrix(osg::Matrix::translate(tX, tY, tZ)); }
inline void theatreScale(osg::ref_ptr<osg::MatrixTransform> scale) { scale->setMatrix(osg::Matrix::scale(tS, tS, tS)); }

const string markerDir = "resources/markers/";
const string modelDir = "resources/models/";

int _width = 640, _height = 480;

void initOGL(int argc, char **argv);
void render(IplImage* frame_input, std::vector<MarkerTransform> mt);

int main(int argc, char **argv) {  
	gnucap_lib::GnucapController circuit(FPS, STEPS);

#ifdef GNUCAP_TESTING
	circuit.test();
	return 0;
#else

	libconfig::Config cfg;
	try {
		cfg.readFile("ar-circuits.cfg");
	} catch (const libconfig::ConfigException &e) {
		cerr << "Could not open config file: " << e.what() << endl;
		return -1;
	}

	try {
		string name = cfg.lookup("printme");
		std::cout << name << endl;
	} catch (libconfig::ConfigException &e) {
		cerr << "Value not found in ar-circuits.cfg - " << e.what() << endl;
		return -1;
	}

	unsigned int i;
	Registration *registration = new RegistrationARToolkit();

	initOGL(argc, argv);

	Capture *camera = new Camera(0, cvSize(320,240), "resources/camera.yml");
	fgCamera->setProjectionMatrix(osg::Matrix(calcProjection(camera->getParameters(), camera->getDistortion(), cvSize(camera->getWidth(), camera->getHeight()))));
	
	int boxHL = 5;
	osg::Geode* boxGeode = new osg::Geode();
	boxGeode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0,0,0), boxHL, boxHL/2, boxHL/2)));

	scenes.push_back(new ARScene(boxGeode, markerDir + "Diode.patt", registration));

	scenes.push_back(new ARScene(modelDir + "cessna.osg", markerDir + "patt.Hiro", registration));
	scenes.push_back(new ARScene(modelDir + "Switch_anim.osg", markerDir + "ACSupply.patt", registration));

	osg::AnimationPath* ap = new osg::AnimationPath();
	vector<osg::Vec3d> points;
	points.push_back(osg::Vec3d(10,0,0));
	points.push_back(osg::Vec3d(0,10,0));
	points.push_back(osg::Vec3d(-10,0,0));
	points.push_back(osg::Vec3d(0,-10,0));
	points.push_back(osg::Vec3d(10,0,0));
	for (i = 0; i < points.size(); i++) 
		ap->insert(i, osg::AnimationPath::ControlPoint(points.at(i), osg::Quat(i*osg::PI/2, osg::Vec3d(0,0,1))));
	ap->setLoopMode(osg::AnimationPath::LOOP);


	scenes.at(1)->setSceneTransformCallback(new osg::AnimationPathCallback(ap));
	scenes.at(0)->setSceneTransformCallback(new osg::AnimationPathCallback(ap));

	for (i = 0; i < scenes.size(); i++){
		fgCamera->addChild(scenes.at(i));
	}

	osg::Node* animationNode = findNamedNode("Switch_animation", fgCamera);
	int keyFrame = 0;

	bool running = true;
	while (running) {
		
		if(animationNode != false){
			osg::NodeCallback* cb = animationNode->getUpdateCallback();
			keyFrame = keyFrame? 0 : 0.5;
			//dynamic_cast<osg::AnimationPathCallback *>(cb)->getAnimationPath()->set //setTimeOffset(keyFrame);
		}


		IplImage *frame = camera->getFrame();
		std::vector<MarkerTransform> mt = registration->performRegistration(frame, camera->getParameters(), camera->getDistortion());
		render(frame, mt);

		switch(cvWaitKey(20)) {
			case 27:
				running = false; break;
		//	case 'q':
		//		tX--; theatreTrans(tOsgMatTrans); break;
		//	case 'a':
		//		tX++; theatreTrans(tOsgMatTrans); break;
		//	case 'w':
		//		tY--; theatreTrans(tOsgMatTrans); break;
		//	case 's':
		//		tY++; theatreTrans(tOsgMatTrans); break;
		//	case 'e':
		//		tZ--; theatreTrans(tOsgMatTrans); break;
		//	case 'd':
		//		tZ++; theatreTrans(tOsgMatTrans); break;
		//	case 't':
		//		tS++; theatreScale(tOsgMatScale); break;
		//	case 'g':
		//		tS--; theatreScale(tOsgMatScale); break;
		//	case ' ':
		//		printf("%.2f\t%.2f\t%.2f\t%.2f\n", tX, tY, tZ, tS);
		}

		cvReleaseImage(&frame);
		for (i=0; i<mt.size(); i++) 
			mt.at(i).clear(); 
		mt.clear();
	}

//	delete registration;
	delete camera;
	return 0;
#endif
}


void initOGL(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(_width, _height);
	glutCreateWindow("SimpleTest");

    viewer.setUpViewerAsEmbeddedInWindow(0,0,_width,_height);
	viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
	viewer.setKeyEventSetsDone(0);

	osg::ref_ptr<osg::Group> root = new osg::Group();
	viewer.setSceneData(root.get());
    viewer.realize();

	mVideoImage = new osg::Image();

	// ----------------------------------------------------------------
	// Video background
	// ----------------------------------------------------------------
	osg::ref_ptr<osg::Camera> bgCamera = new osg::Camera();
	bgCamera->getOrCreateStateSet()->setRenderBinDetails(0, "RenderBin");
	bgCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	bgCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
	bgCamera->getOrCreateStateSet()->setMode(GL_LIGHTING, GL_FALSE);
	bgCamera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, GL_FALSE);
	bgCamera->setProjectionMatrixAsOrtho2D(0, _width, 0, _height);

	osg::ref_ptr<osg::Geometry> geomBG = osg::createTexturedQuadGeometry(osg::Vec3(0, 0, 0), osg::X_AXIS * _width, osg::Y_AXIS * _height, 0, 1, 1, 0);
	geomBG->getOrCreateStateSet()->setTextureAttributeAndModes(0, new osg::Texture2D(mVideoImage));

	osg::ref_ptr<osg::Geode> geodeBG = new osg::Geode();
	geodeBG->addDrawable(geomBG.get());
	bgCamera->addChild(geodeBG.get());
	root->addChild(bgCamera.get());

	// ----------------------------------------------------------------
	// Foreground 3D content
	// ----------------------------------------------------------------
	fgCamera = new osg::Camera();
	fgCamera->getOrCreateStateSet()->setRenderBinDetails(1, "RenderBin");
	fgCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF_INHERIT_VIEWPOINT);
	fgCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
	
	root->addChild(fgCamera.get());
}


void render(IplImage* frame_input, std::vector<MarkerTransform> mt) { 
	unsigned int i, j;
	//Copy the frame into the background image
	IplImage *scaleImage = cvCreateImage(cvSize(512,512), IPL_DEPTH_8U, 3);
	cvResize(frame_input, scaleImage); cvCvtColor(scaleImage, scaleImage, CV_RGB2BGR);
 	mVideoImage->setImage(scaleImage->width, scaleImage->height, 0, 3, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)scaleImage->imageData, osg::Image::NO_DELETE);
	
	for (j = 0; j < scenes.size(); j++){
		scenes.at(j)->setVisibility(false);  //TODO: add safety for when no markers are added
	}

	for (i=0; i<mt.size(); i++) {
		for (j = 0; j < scenes.size(); j++){
			if (mt.at(i).marker.name == scenes.at(j)->getMarkerID()) {
				scenes.at(j)->setVisibility(true);
				scenes.at(j)->setTransform(osg::Matrix(mt.at(i).transMat));
			}
		}
	}

	viewer.frame();

	IplImage* outImage = cvCreateImage(cvSize(_width,_height), IPL_DEPTH_8U, 3);
	glReadPixels(0,0,_width,_height,GL_RGB, GL_UNSIGNED_BYTE, outImage->imageData);
	cvCvtColor( outImage, outImage, CV_BGR2RGB );
	cvFlip(outImage, outImage);

	cvShowImage("Rendered Image", outImage);
	cvReleaseImage(&outImage);

	cvReleaseImage(&scaleImage);
}