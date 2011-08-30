#include "StdAfx.h"
#include "RegistrationARToolkit.h"
#include "ARScene.h"


using namespace OPIRALibrary;

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

class modelOSG: public osg::Group {
public:
	osg::ref_ptr<osg::Node> model;
	osg::ref_ptr<osg::MatrixTransform> transform;
	osg::ref_ptr<osg::MatrixTransform> osgTransform;
	osg::ref_ptr<osg::Switch> visible;
	
	modelOSG(osg::Node *n) {
		model = n;
		transform = new osg::MatrixTransform(osg::Matrix::identity());
		osgTransform = new osg::MatrixTransform(osg::Matrix::identity());//osg::Matrix::rotate(osg::DegreesToRadians(180.0f), osg::X_AXIS));
		visible = new osg::Switch();
		visible->addChild(transform); transform->addChild(osgTransform); osgTransform->addChild(model); transform->addChild(createLights(this));
		this->addChild(visible);
	};
};


osgViewer::Viewer viewer;
osg::ref_ptr<osg::Image> mVideoImage;
osg::ref_ptr<osg::Camera> fgCamera;
osg::ref_ptr<modelOSG> batMask; osg::ref_ptr<modelOSG> theatreMask;
vector<ARScene *> scenes;

void initOGL(int argc, char **argv);
void render(IplImage* frame_input, std::vector<MarkerTransform> mt);

int _width = 640, _height = 480;

float bX=44, bY=142, bZ=113, bS=310;
float tX=17, tY=232, tZ=17, tS=8;

inline void theatreTrans(osg::ref_ptr<osg::MatrixTransform> trans) { trans->setMatrix(osg::Matrix::translate(tX, tY, tZ)); }
inline void theatreScale(osg::ref_ptr<osg::MatrixTransform> scale) { scale->setMatrix(osg::Matrix::scale(tS, tS, tS)); }

vector<string> markerPaths(0);
const string markerDir = "resources/markers/";
const string modelDir = "resources/models/";

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

void main(int argc, char **argv) {  

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
	ap->insert(0, osg::AnimationPath::ControlPoint(osg::Vec3d(10, 0, 0), osg::Quat(0, osg::Vec3d(0,0,1))));
	ap->insert(1, osg::AnimationPath::ControlPoint(osg::Vec3d(0, 10, 0), osg::Quat(osg::PI/2, osg::Vec3d(0,0,1))));
	ap->insert(2, osg::AnimationPath::ControlPoint(osg::Vec3d(-10, 0, 0), osg::Quat(osg::PI, osg::Vec3d(0,0,1))));
	ap->insert(3, osg::AnimationPath::ControlPoint(osg::Vec3d(0, -10, 0), osg::Quat(osg::PI*3/2, osg::Vec3d(0,0,1))));
	ap->insert(4, osg::AnimationPath::ControlPoint(osg::Vec3d(10, 0, 0), osg::Quat(osg::PI*2, osg::Vec3d(0,0,1))));
	ap->setLoopMode(osg::AnimationPath::LOOP);
	scenes.at(1)->setSceneTransformCallback(new osg::AnimationPathCallback(ap));
	scenes.at(0)->setSceneTransformCallback(new osg::AnimationPathCallback(ap));

	for (int i = 0; i < scenes.size(); i++){
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
		for (int i=0; i<mt.size(); i++) mt.at(i).clear(); mt.clear();
	}

//	delete registration;
	delete camera;
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
	//Copy the frame into the background image
	IplImage *scaleImage = cvCreateImage(cvSize(512,512), IPL_DEPTH_8U, 3);
	cvResize(frame_input, scaleImage); cvCvtColor(scaleImage, scaleImage, CV_RGB2BGR);
 	mVideoImage->setImage(scaleImage->width, scaleImage->height, 0, 3, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)scaleImage->imageData, osg::Image::NO_DELETE);
	
	for (int j = 0; j < scenes.size(); j++){
		scenes.at(j)->setVisibility(false);  //TODO: add safety for when no markers are added
	}

	for (int i=0; i<mt.size(); i++) {
		for (int j = 0; j < scenes.size(); j++){
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