#pragma once

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "Utils.h"

using namespace cv;

/**
 * The Camera class consists of two video/camera sequences
 */
class Camera
{
	VideoCapture cam[2];
	Mat frame[2], tmpFrame[2], edgeFrame[2], startFrame[2];

	Mat edgeFilterLR;
	Mat edgeFilterRL;

public:
	Camera(char *fn1, char *fn2);
	~Camera(void);

	bool checkCameras();

	void startSequence();
	void showNextFrame(int i);

	VideoCapture getCamera(int i);
	Mat getNextFrame(int i);
};