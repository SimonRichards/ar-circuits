#include "StdAfx.h"

#include "ARToolkit2.7OpenCV.h"

using namespace OPIRALibrary;

RegistrationARToolkit::RegistrationARToolkit(CvMat* captureParams, CvMat* captureDistortion)
	: captureParams(captureParams), captureDistortion(captureDistortion)
{
	cParam = 0;
}

RegistrationARToolkit::RegistrationARToolkit(const char *pattern_name) {
	//Set the ARToolkit Camera Params to 0
	cParam=0;

	addMarker(pattern_name, 80, 80);

}

RegistrationARToolkit::RegistrationARToolkit(std::vector<const char *> pattern_names) {
	//Set the ARToolkit Camera Params to 0
	cParam=0;

	for (unsigned int i=0; i<pattern_names.size(); i++ ){
		addMarker(pattern_names.at(i), 80, 80);
	}
}

bool RegistrationARToolkit::addMarker(string markerName, int width, int height) {
	//Create a new marker for the pattern
	Marker m;
	m.name = markerName;
	m.size.width = width; m.size.height = height;

	//Load the pattern
	int pattID = (arLoadPatt(markerName.c_str()));
	if(pattID < 0 ) {
        printf("pattern load error !!\n");
		return false;
	}
	
	//Add the marker to the list
	markers.push_back(m);

	return true;
}

RegistrationARToolkit::~RegistrationARToolkit() {
	//Clean up afterwards
	delete cParam;
}

std::vector<MarkerTransform> RegistrationARToolkit::performRegistration(IplImage* frame_input) {
	//Our return MarkerTransform array
	std::vector<MarkerTransform> retVal;

	//ARToolkit marker info and marker number count
	ARMarkerInfo    *marker_info;
	int             marker_num;

	//If the camera isn't initialised yet
	if (cParam==0) {
		//Initialise it6
		cParam = convertCameraParams(captureParams, captureDistortion, frame_input->width, frame_input->height);
		//And initialise the ARToolkit
		arInitCparam( cParam );	arParamDisp( cParam );
	}

	//ARToolkit by default assumes BGRA image format, OpenCV delivers RGB, so we have to convert
	IplImage *bgraImage = cvCreateImage( cvGetSize(frame_input), IPL_DEPTH_8U, 4 );
	cvCvtColor(frame_input, bgraImage, CV_RGB2BGRA);

	//Find Markers
	arDetectMarkerConf((ARUint8 *)bgraImage->imageData, 100, &marker_info, &marker_num, 0.5);

	//For all Markers
	for (int i=0; i<marker_num; i++) {
		//marker_info[i].id = 0;
		if (marker_info[i].id>-1) {
			MarkerTransform mt;

			//Set up our MarkerTransform object		
			mt.marker = markers.at(marker_info[i].id);
			mt.viewPort = calcViewpoint(captureParams, captureDistortion, cvGetSize(frame_input));
			mt.projMat = calcProjection(captureParams, captureDistortion, cvGetSize(frame_input));
			calcAROpenGLTransform(marker_info[i].dir, marker_info[i].vertex, captureParams, captureDistortion, &mt.transMat, mt.marker);
			mt.score = marker_info[i].cf; 		

			//Add the MarkerTransform to the list
			retVal.push_back(mt);		
		}
	}

	//Clean up
	cvReleaseImage(&bgraImage);

	//Return the array of marker transforms
	return retVal;
}

//Convert OpenCV Camera Parameters into ARToolkit Camera Parameters
ARParam* RegistrationARToolkit::convertCameraParams(CvMat* captureParams, CvMat* captureDistortion, int captureWidth, int captureHeight) {
	//Allocate Space
	ARParam *cparam = (ARParam*)malloc(sizeof(ARParam));
	//Copy the Width and Size
	cparam->xsize = captureWidth; cparam->ysize = captureHeight;
	
	//The Distortion Factor appears to be width/2, height/2, the next 2 values are near and far perhaps?
	cparam->dist_factor[0] = cparam->xsize/2;
	cparam->dist_factor[1] = cparam->ysize/2;
	cparam->dist_factor[2] = 100;
	cparam->dist_factor[3] = 1;

	//If the camera has distortion parameters, modify the ARToolkit parameters
	CvMat* camDist = captureDistortion;
	if (camDist!=0) {
		cparam->dist_factor[0] += camDist->data.db[0];
		cparam->dist_factor[1] += camDist->data.db[1];
		cparam->dist_factor[2] += camDist->data.db[2];
		cparam->dist_factor[3] += camDist->data.db[3];
	}

	//Copy the camera parameters over
	CvMat* camParam = captureParams;
	cparam->mat[0][0] = camParam->data.db[0]; 	cparam->mat[0][1] = camParam->data.db[1];	cparam->mat[0][2] = camParam->data.db[2];	cparam->mat[0][3] = 0;
	cparam->mat[1][0] = camParam->data.db[3];	cparam->mat[1][1] = camParam->data.db[4];	cparam->mat[1][2] = camParam->data.db[5];	cparam->mat[1][3] = 0;
	cparam->mat[2][0] = camParam->data.db[6];	cparam->mat[2][1] = camParam->data.db[7];	cparam->mat[2][2] = camParam->data.db[8];	cparam->mat[2][3] = 0;

	return cparam;
}

/*Process Matches to find Extrinsic Camera Parameters*/
void RegistrationARToolkit::calcAROpenGLTransform(int direction, double vertex[4][2], CvMat* captureParams, CvMat* captureDistortion, double *transMat[16],
	Marker marker)
{
		//Known points on the object we're tracking
		CvMat*	objectPoints = cvCreateMat( 4, 3, CV_32FC1 );

		//Found points in the scene
		CvMat* imagePoints = cvCreateMat(4,2, CV_32FC1);
		
		float halfX = marker.size.width/2;
		float halfY = marker.size.height/2;

		objectPoints->data.fl[0] = -halfX; objectPoints->data.fl[1] = -halfY; objectPoints->data.fl[2] = 0;
		objectPoints->data.fl[3] = halfX; objectPoints->data.fl[4] = -halfY; objectPoints->data.fl[5] = 0;
		objectPoints->data.fl[6] = halfX; objectPoints->data.fl[7] = halfY; objectPoints->data.fl[8] = 0;
		objectPoints->data.fl[9] = -halfX; objectPoints->data.fl[10] = halfY; objectPoints->data.fl[11] = 0;

		imagePoints->data.fl[direction*2] = vertex[0][0]; imagePoints->data.fl[(direction*2)+1] = vertex[0][1];
		imagePoints->data.fl[((direction+1)%4)*2] = vertex[1][0]; imagePoints->data.fl[(((direction+1)%4)*2)+1] = vertex[1][1];
		imagePoints->data.fl[((direction+2)%4)*2] = vertex[2][0]; imagePoints->data.fl[(((direction+2)%4)*2)+1] = vertex[2][1];
		imagePoints->data.fl[((direction+3)%4)*2] = vertex[3][0]; imagePoints->data.fl[(((direction+3)%4)*2)+1] = vertex[3][1];

		//Set up the OpenCV transformation matrix as an identity matrix
		CvMat *cvTransMat = cvCreateMat(4,4, CV_32FC1); cvSetIdentity(cvTransMat);

		//Set the translation vector and rotation matrix to point inside the cvTransMat struct
		CvMat *transVector = cvCreateMatHeader(1, 3, CV_32F); cvGetSubRect(cvTransMat, transVector, cvRect(3,0,1,3));
		CvMat *rotMat = cvCreateMatHeader(3,3, CV_32FC1); cvGetSubRect(cvTransMat, rotMat, cvRect(0,0,3,3));
	
		//Find the Extrinsic Camera Parameters, and convert the rotation vector to the rotation matrix
		CvMat *rotVector= cvCreateMat(1, 3, CV_32F); 
		cvFindExtrinsicCameraParams2(objectPoints, imagePoints, captureParams, captureDistortion, rotVector, transVector);
		cvRodrigues2(rotVector, rotMat);
	
		//Clean up
		cvReleaseMat(&rotVector);
		cvReleaseMatHeader(&transVector); cvReleaseMatHeader(&rotMat);

		*transMat = calcTransform(cvTransMat);

		cvReleaseMat(&cvTransMat);

		//Cleanup
		cvReleaseMat(&objectPoints);
		cvReleaseMat(&imagePoints);
	}