#pragma once

#include <windows.h>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/io_utils>
#include <GL/glut.h>
//OPIRA
#include <CaptureLibrary.h>
#include <OPIRALibrary.h>
#include <AR\ar.h>
#include <libconfig.h++>

#include <string>
#include <iostream>
#include <sstream>


static string markerDir = "resources/markers/";
static string modelDir = "resources/models/";