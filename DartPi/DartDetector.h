#pragma once

#include "Camera.h"

#include <vector>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

using namespace cv;

#define ADAPTION_RATE 0.2

class DartDetector
{
	Camera *camera;
	Mat background[2];
	Mat currentFrame[2];

	int frameDetection[2];
	int maximumDifference[2];

	bool initialized;
//	static double adaptionRate = 0.1;
	Mat edgeFilterH;
	Mat edgeFilterV;

	Mat output[2];

	struct {
		bool hasChanged;
		vector< vector<Point> > contours;
	} dartcontour[2];

	struct {
		bool hasChanged;
		char *text;
	} darttext[2];

	struct {
		bool hasChanged;
		Rect board;
	} dartboard[2];

	struct {
		bool isSet;
		Point2d pnt;
		int score;
	} lastScore;

	int dartCnt[2];

	double radii[6];
	int scores[20];

	vector< vector<Point> > boardContours;

public:
	DartDetector(Camera *cam);
	~DartDetector(void);

	void beginSequence();

	int getNextShoot();

	void renderScene(int cameraNo);

	void renderVisualization();

private:
	void initializeShoot(int cameraNo);
	camera_detection analyzeFrame(int cameraNo, int frameNr);
	void initializeDartboard(int camerNo, Mat image);

	double calculateDartCoordinate(int cameraNo);
	void calculateDartScore();

};

