#ifndef ARTOOLKIT2POINT7OPENCV_H
#define ARTOOLKIT2POINT7OPENCV_H

#include "OPIRALibrary.h"
#include <AR\ar.h>
#include <string>

namespace OPIRALibrary {

class RegistrationARToolkit: public Registration {
	public:
		RegistrationARToolkit(CvMat* captureParams, CvMat* captureDistortion);
		RegistrationARToolkit(const char* pattern_name);
		RegistrationARToolkit(std::vector<const char *> pattern_names);

		std::vector<MarkerTransform> performRegistration(IplImage* frame_input);
		bool addMarker(string markerName, int width, int height);	



		~RegistrationARToolkit();

	private:
		ARParam *cParam;
		CvMat* captureParams;
		CvMat* captureDistortion;
		std::vector<int> markerPattNum;

		ARParam* convertCameraParams(CvMat* captureParams, CvMat* captureDistortion, int captureWidth, int captureHeight);
		void calcAROpenGLTransform(int direction, double vertex[4][2], CvMat* captureParams, CvMat* captureDistortion, double *transMat[16], Marker marker);
	};
}

#endif