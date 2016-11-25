// DartPi.cpp : Defines the entry point for the console application.
// DartBoard radii: 12.7, 31.8, 99.0, 107.0, 162.0, 170.0mm; outer diameter = 450mm

#include <stdio.h>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "Camera.h"
#include "DartDetector.h"
#include "Lcd5110.h"

#ifndef DEBUG
#define DEBUG 0
#endif


using namespace cv;


int main(int argc, char* argv[])
{
	//printf("DartScoreDetection by Christian Hirsch (0825187)\n");
	//printf("------------------------------------------------\n");

	//VideoCapture cap(0) //webcam

	Camera dartcam("data/Video1_Cam1.avi", "data/Video1_Cam2.avi");
	if(!dartcam.checkCameras())
		return -1;

	DartDetector dartdetector(&dartcam);

	dartcam.getNextFrame(0);
	dartcam.getNextFrame(1);

	dartdetector.renderScene(0);
	dartdetector.renderScene(1);
	dartdetector.renderVisualization();

	// init LCD screen
	//GPIO_CLR = 1 << LIGHT;
	//initLcdPorts();
	//initLcdScreen();

	//clearLcdScreen();
	//writeStringToLcdXY(0, 0, "Press a key to start ...");
	//printf("Press a key to start ...");
	waitKey(0);
	//clearLcdScreen();
	//writeStringToLcdXY(0, 0, "DartPi Testing");
	//writeStringToLcdXY(0, 1, "--------------");
	//printf(" started!\n");

	for(int i=1;;i++)
	{
		int score = dartdetector.getNextShoot();
		char lcdText[14];
		sprintf(lcdText, "[%03d] Score: %2d", i, score);
		//writeStringToLcdXY(0, 3, lcdText);
		printf("%s\n", lcdText);
		dartdetector.renderScene(0);
		dartdetector.renderScene(1);
		dartdetector.renderVisualization();
		if(waitKey(1) > 0)
			break;
	}
	return 0;
}
