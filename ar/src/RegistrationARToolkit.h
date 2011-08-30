#pragma once

#include "StdAfx.h"

namespace OPIRALibrary {

class RegistrationARToolkit: public Registration {
	public:
		RegistrationARToolkit();
		RegistrationARToolkit(const char* pattern_name);
		RegistrationARToolkit(std::vector<const char *> pattern_names);

		std::vector<MarkerTransform> performRegistration(IplImage* frame_input, CvMat* captureParams, CvMat* captureDistortion);
		bool addMarker(string markerName);		

		~RegistrationARToolkit();

	private:
		ARParam *cParam;
		ARParam* convertCameraParams(CvMat* captureParams, CvMat* captureDistortion, int captureWidth, int captureHeight);
		std::vector<int> markerPattNum;
		void calcAROpenGLTransform(int direction, double vertex[4][2], CvMat* captureParams, CvMat* captureDistortion, double *transMat[16]);
	};
}
