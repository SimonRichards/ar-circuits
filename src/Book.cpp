#include "StdAfx.h"

#include "Book.h"

Book::Book(const libconfig::Setting& config, int argc, char** argv)
	: camWidth(800), camHeight(600), gnucap(20, 5)
{
 	initOGL(argc, argv);
    
	camera = new CvCamera(0, "resources/camera.yml");
	fgCamera->setProjectionMatrix(osg::Matrix(calcProjection(camera->getParameters(), camera->getDistortion(), cvSize(camera->getWidth(), camera->getHeight()))));
	

	reg = new RegistrationARToolkit(camera->getParameters(), camera->getDistortion());

    

	for(int i = 0; i < config["circuits"].getLength(); i++){
		ARCircuits.push_back(new ARCircuit(config["circuits"][i], gnucap, reg));
	}	
	for each (ARCircuit* ARCircuit in ARCircuits){
		for each (ARScene* scene in ARCircuit->getScenes()){
			fgCamera->addChild(scene);
		}
	}
}

Book::~Book(void)
{
    fgCamera.release();
    //reg->
    delete camera;
    //delete reg;
}

vector<ARScene*> Book::getScenes(int ARCircuitNum){
	return ARCircuits.at(ARCircuitNum)->getScenes();
}

void Book::run(){
	int frameMult = 1;
	bool running = true;
	bool pauseNext = false;
	while (running) {
		IplImage *frame = camera->getFrame();
        //tic();
		std::vector<MarkerTransform> mt = reg->performRegistration(frame);
        //toc("performing registration");

        //tic();
		render(frame, mt);
        //toc("rendering");

		if(pauseNext){
			pauseNext = false;
		}
		switch(cvWaitKey(1)) {
			case 27:
				running = false; 
				break;
			case 'q':
				break;
		}
        gnucap.analyse();
        
		cvReleaseImage(&frame);
		for (unsigned int i=0; i<mt.size(); i++) mt.at(i).clear(); mt.clear();
	}
}

void Book::initOGL(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(camWidth, camHeight);
	glutCreateWindow("SimpleTest");

    viewer.setUpViewerAsEmbeddedInWindow(0,0,camWidth,camHeight);
	viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
	viewer.setKeyEventSetsDone(0);
    // add the window size toggle handler
	osgViewer::WindowSizeHandler* full = new osgViewer::WindowSizeHandler;
    viewer.addEventHandler(full);
	//full->setToggleFullscreen(true);

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
	bgCamera->setProjectionMatrixAsOrtho2D(0, camWidth, 0, camHeight);

	osg::ref_ptr<osg::Geometry> geomBG = osg::createTexturedQuadGeometry(osg::Vec3(0, 0, 0), osg::X_AXIS * camWidth, osg::Y_AXIS * camHeight, 0, 1, 1, 0);
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

void Book::render(IplImage* frame_input, std::vector<MarkerTransform> mt) { 
	//Copy the frame into the background image
	IplImage *scaleImage = cvCreateImage(cvSize(512,512), IPL_DEPTH_8U, 3);
	cvResize(frame_input, scaleImage); cvCvtColor(scaleImage, scaleImage, CV_RGB2BGR);
 	mVideoImage->setImage(scaleImage->width, scaleImage->height, 0, 3, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)scaleImage->imageData, osg::Image::NO_DELETE);
	
	for each (ARCircuit* ARCircuit in ARCircuits){
		for each (ARScene* scene in ARCircuit->getScenes()){
			scene->setMarkerVisible(false);  //TODO: add safety for when no markers are added
		}
	}

	for each (MarkerTransform transform in mt) {
		for each (ARCircuit* ARCircuit in ARCircuits){
			for each (ARScene* scene in ARCircuit->getScenes()){
				if (transform.marker.name == scene->getMarkerID()){// && transform.score > 5) {
					scene->setMarkerVisible(true);
					scene->setMarkerMatrix(osg::Matrix(transform.transMat));
				}
			}
		}
	}

	for each (ARCircuit* ARCircuit in ARCircuits){
		ARCircuit->updateARCircuit();
	}
    
	viewer.frame();

	IplImage* outImage = cvCreateImage(cvSize(camWidth,camHeight), IPL_DEPTH_8U, 3);
	glReadPixels(0,0,camWidth,camHeight,GL_RGB, GL_UNSIGNED_BYTE, outImage->imageData);
	cvCvtColor( outImage, outImage, CV_BGR2RGB );
	cvFlip(outImage, 0, 0);

	cvShowImage("Rendered Image", outImage);
	cvReleaseImage(&outImage);

	cvReleaseImage(&scaleImage);
}