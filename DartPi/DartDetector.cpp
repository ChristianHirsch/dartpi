#include <cmath>

#include "DartDetector.h"
#include "Utils.h"

#define PI 3.14159265

std::vector<int> getKMaxValueIndices(Mat values, int k)
{
	int sz = values.cols > values.rows ? values.cols : values.rows;

	if(values.cols > values.rows)
		values = values.t();

	std::vector<double> maxValues(k, 0.0);
	std::vector<int> maxIndices(k, 0);

	for(int i=0; i<sz; i++)
	{
		double curValue = values.at<double>(i,0);
		double minValue = maxValues.back();

		if(curValue < 0.1)
			continue;

		int istart = i;
		double maxCurValue = 10.0*curValue;
		while(i < sz-1
			&& values.at<double>(i+1,0) > 0.1)
		{
			i++;
			curValue = values.at<double>(i,0);
			if(curValue > maxCurValue)
				maxCurValue = curValue;
		}
		maxCurValue = i-istart;

		if(maxCurValue < maxValues.back())
			continue;

		// insert new value at propriate point
		for(int j=0; j<k; j++)
		{
			if(maxCurValue < maxValues.at(j))
				continue;

			maxValues.insert(maxValues.begin() + j, maxCurValue);
			maxIndices.insert(maxIndices.begin() + j, istart + (i-istart)/2);
			break;
		}

		// delete smallest value
		maxValues.pop_back();
		maxIndices.pop_back();
	}

	sort(maxIndices.begin(), maxIndices.end());
	return maxIndices;
}

DartDetector::DartDetector(Camera *cam)
{
	camera = cam;
	initialized = false;

	edgeFilterH = Mat(1, 3, CV_64F);
	edgeFilterV = Mat(3, 1, CV_64F);

	edgeFilterH.at<double>(0,0) =  1.0;
	edgeFilterH.at<double>(0,1) =  0.0;
	edgeFilterH.at<double>(0,2) = -1.0;

	edgeFilterV.at<double>(0,0) =  1.0;
	edgeFilterV.at<double>(1,0) =  0.0;
	edgeFilterV.at<double>(2,0) = -1.0;

	lastScore.isSet = false;
	lastScore.pnt = Point2d(0.0, 0.0);

	radii[0] = 12.7/2.0; radii[1] = 31.8/2.0; radii[2] = 99.0;
	radii[3] = 107.0; radii[4] = 162.0; radii[5] = 170.0;

	scores[0] = 1;	scores[1] = 18;	scores[2] = 4; 
	scores[3] = 13;	scores[4] = 6;	scores[5] = 10; 
	scores[6] = 15;	scores[7] = 2;	scores[8] = 17; 
	scores[9] = 3;	scores[10] = 19;	scores[11] = 7; 
	scores[12] = 16;	scores[13] = 8;	scores[14] = 11; 
	scores[15] = 14;	scores[16] = 9;	scores[17] = 12; 
	scores[18] = 5;	scores[19] = 20;

	int cameraNo = 0;
	darttext[cameraNo].hasChanged = dartboard[cameraNo].hasChanged
		= dartcontour[cameraNo].hasChanged = false;
	dartCnt[cameraNo] = 0;
	darttext[cameraNo].text = (char *) malloc(sizeof(char) * 256);

	cameraNo = 1;
	darttext[cameraNo].hasChanged = dartboard[cameraNo].hasChanged
		= dartcontour[cameraNo].hasChanged = false;
	dartCnt[cameraNo] = 0;
	darttext[cameraNo].text = (char *) malloc(sizeof(char) * 256);

	initializeShoot(0);
	initializeShoot(1);
}


DartDetector::~DartDetector(void)
{
}


void DartDetector::beginSequence()
{
	Mat frame = camera->getNextFrame(0);
}

void DartDetector::initializeShoot(int cameraNo)
{
	printf("Initializing...");

	// calibrating (remove background, check dart borders)
	if(!initialized)
	{
		background[cameraNo] = camera->getNextFrame(cameraNo);

		currentFrame[cameraNo] = camera->getNextFrame(cameraNo);
		background[cameraNo] = (1-ADAPTION_RATE)*background[cameraNo]
			+ ADAPTION_RATE*currentFrame[cameraNo];
		
		Mat BWdifference;
		absdiff(background[cameraNo], currentFrame[cameraNo], BWdifference);
		threshold(BWdifference, BWdifference, 30, 255, CV_THRESH_BINARY);
		
		double difference = sum(BWdifference)[0];

		while(difference > 0.0)
		{
			currentFrame[cameraNo] = camera->getNextFrame(cameraNo);
			background[cameraNo] = (1-ADAPTION_RATE)*background[cameraNo]
				+ ADAPTION_RATE*currentFrame[cameraNo];
			absdiff(background[cameraNo], currentFrame[cameraNo], BWdifference);
			threshold(BWdifference, BWdifference, 30, 255, CV_THRESH_BINARY);
			difference = sum(BWdifference)[0];
		}
	}

	frameDetection[cameraNo] = 0;
	maximumDifference[cameraNo] = 0;

	printf(" finished!\n");
}

camera_detection DartDetector::analyzeFrame(int cameraNo, int frameNr)
{
	currentFrame[cameraNo] = camera->getNextFrame(cameraNo);
			
	Mat BWdifference;
	absdiff(background[cameraNo], currentFrame[cameraNo], BWdifference);
	threshold(BWdifference, BWdifference, 30, 255, CV_THRESH_BINARY);
	
	background[cameraNo] = (1-ADAPTION_RATE)*background[cameraNo]
		+ ADAPTION_RATE*currentFrame[cameraNo];
	
	int difference = sum(BWdifference)[0];

	if(difference < CAMERA_WIDTH * CAMERA_HEIGHT * 0.01)
		return nothing;

	if(difference > CAMERA_WIDTH * CAMERA_HEIGHT * 0.5)
		return change;

	// EXTRACT DART
	maximumDifference[cameraNo] = difference;
	morphologyEx(BWdifference, BWdifference, MORPH_CLOSE,
		getStructuringElement(MORPH_ELLIPSE, Size(7,21)), Point(-1,-1), 3);
	findContours(BWdifference,
		dartcontour[cameraNo].contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	dartcontour[cameraNo].hasChanged = true;

	return dart;

/*
	int frameDifference = frameNr - frameDetection[cameraNo];
	if(frameDifference > 1)
	{				
		if(frameDifference < 11)
			dartCnt[cameraNo]++;
		else
			dartCnt[cameraNo] = 0;
#if DEBUG
		printf("%d | %d | %s - %d \t (diff | frame diff | type - nr)\n", difference, frameDifference,
				(frameDifference > 10 ? "change" : "dart"), dartCnt[cameraNo]);
#endif
#if SHOW_WINDOWS
		sprintf(darttext[cameraNo].text, "%s [%d]",
			(frameDifference > 10 ? "change" : "dart"), dartCnt[cameraNo]);
		//printf("%s\n", darttext[cameraNo].text);

		darttext[cameraNo].hasChanged = true;
#endif
		maximumDifference[cameraNo] = 0;

		if(dartCnt[cameraNo] > 0)
			return dart;
		if(frameDifference < 100 && frameDifference > 30)
			return change;
	}
	frameDetection[cameraNo] = frameNr;

		return nothing;
	}
	if(maximumDifference[cameraNo] <= difference)
	{
		// EXTRACT DART
		maximumDifference[cameraNo] = difference;
		morphologyEx(BWdifference, BWdifference, MORPH_CLOSE,
			getStructuringElement(MORPH_ELLIPSE, Size(7,21)), Point(-1,-1), 3);
		findContours(BWdifference,
			dartcontour[cameraNo].contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		dartcontour[cameraNo].hasChanged = true;
	}

	return nothing;
	*/
}

int DartDetector::getNextShoot()
{
	initializeShoot(0);
	initializeShoot(1);

	camera_detection camDetection[2] { nothing, nothing };
	int camDetectionFrameNo[2] = { 0, 0 };
	bool exit[2] = { false, false };

	for(int frameNo = 1;; frameNo++)
	{
		camera_detection tmpDet[2];
		tmpDet[0] = analyzeFrame(0, frameNo);
		tmpDet[1] = analyzeFrame(1, frameNo);
		
		if(tmpDet[0] != nothing)
		{
			if(camDetectionFrameNo[0] < 1)
				camDetectionFrameNo[0] = frameNo;
			if(frameNo - camDetectionFrameNo[0] < 3)
				camDetection[0] = tmpDet[0];
		} else if (camDetectionFrameNo[0] > 0)
		{
			if(exit[1])
				break;
			exit[0] = true;

			if(camDetectionFrameNo[1] < 1
				&& (frameNo - camDetectionFrameNo[0]) > 5)
				break;
		}

		if(tmpDet[1] != nothing)
		{
			if(camDetectionFrameNo[1] < 1)
				camDetectionFrameNo[1] = frameNo;
			if(frameNo - camDetectionFrameNo[1] < 3)
				camDetection[1] = tmpDet[1];
		} else if (camDetectionFrameNo[1] > 0)
		{
			if(exit[0])
				break;

			exit[1] = true;

			if(camDetectionFrameNo[0] < 1
				&& (frameNo - camDetectionFrameNo[1]) > 5)
				break;
		}
	}

	lastScore.isSet = false;

	if(camDetection[0] == nothing
		|| camDetection[1] == nothing)
		return -1;

	if(camDetection[0] == camDetection[1]
		&& camDetection[0] == change)
		return 0;

	return 1;

	calculateDartScore();
	return lastScore.score;
}

void DartDetector::initializeDartboard(int cameraNo, Mat image)
{
	Mat original = image.clone();
	Mat edges = image.clone();
	Mat tmp;
	Mat gauss = getGaussianKernel(5, 7, CV_64F);

	// initialize dartboard
	dartboard[cameraNo].board.x = dartboard[cameraNo].board.y = 0;
	dartboard[cameraNo].board.width = original.cols-1;
	dartboard[cameraNo].board.height = original.rows-1;

	// create mask
	threshold(edges, edges, 50, 255, CV_THRESH_BINARY_INV);
	morphologyEx(edges, edges, MORPH_ERODE,
		getStructuringElement(MORPH_RECT, Size(30,10)), Point(-1,-1),
		1, BORDER_CONSTANT, Scalar(0.0));
	
#if DEBUG
	imshow("edges", edges);
#endif

	findContours(edges, boardContours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	int maxIndex = 0;
	for(int i=1; i<boardContours.size(); i++)
		if(contourArea(boardContours[maxIndex]) < contourArea(boardContours[i]))
			maxIndex = i;

	Mat maskedImage(edges.rows, edges.cols, edges.type());
	maskedImage = 1.0;

	Rect contour = boundingRect(boardContours[maxIndex]);
	contour.x = 0;
	contour.width = maskedImage.cols-1;
	
	rectangle(maskedImage, Rect(0.0, 0.0, maskedImage.cols, maskedImage.rows), 255.0, CV_FILLED);
	rectangle(maskedImage, contour, 0.0, CV_FILLED);

	maskedImage = original + maskedImage;

#if DEBUG
	imshow("maskedImage", maskedImage);
#endif

	// get borders
	filter2D(maskedImage, edges, -1, edgeFilterH);
	filter2D(maskedImage, tmp, -1, -1.0*edgeFilterH);
	edges = edges + tmp;
	threshold(edges, edges, 15, 255, CV_THRESH_BINARY);
	Mat cols;
	reduce(edges, cols, 0, CV_REDUCE_SUM, CV_64F);
	filter2D(cols, cols, -1, gauss.t());
	normalize(cols, cols);

	vector<int> columnIndices = getKMaxValueIndices(cols, 2);

	dartboard[cameraNo].board.y = contour.y;
	dartboard[cameraNo].board.height = contour.height;
	dartboard[cameraNo].board.x = columnIndices.front();
	dartboard[cameraNo].board.width = columnIndices.back() - dartboard[cameraNo].board.x;
	dartboard[cameraNo].hasChanged = true;
	/* */
}

void DartDetector::renderScene(int cameraNo)
{
	output[cameraNo] = currentFrame[cameraNo].clone();
	
	cvtColor(output[cameraNo], output[cameraNo], CV_GRAY2BGR);

	if(dartcontour[cameraNo].hasChanged)
	{
		drawContours(output[cameraNo],
			dartcontour[cameraNo].contours, -1, CV_RGB(0.0, 0.0, 255.0), 3);

		bool ok = false;

		int maxContourIndex = 0;
		int maxsize = dartcontour[cameraNo].contours.
			at(maxContourIndex).size();
		for(int i=0; i < dartcontour[cameraNo].contours.size(); i++)
		{
			int maxsize = 0;
			vector<Point> cont = dartcontour[cameraNo].contours.at(i);
			 if(maxsize < cont.size())
			 {
				 maxContourIndex = i;
				 maxsize = cont.size();
				 if(maxsize > 4)
					ok = true;
			 }
		}
		if(ok)
		{
			//RotatedRect rotRect = fitEllipse(
			//	dartcontour[cameraNo].contours.at(maxContourIndex));
			//ellipse(output[cameraNo],
			//	rotRect, CV_RGB(0.0,0.0,255.0), 3);
		}
	}

	if(dartboard[cameraNo].hasChanged)
	{
		rectangle(output[cameraNo],
			dartboard[cameraNo].board, CV_RGB(255.0, 0.0, 0.0), 3);
	}

	if(darttext[cameraNo].hasChanged)
	{
		putText(output[cameraNo], darttext[cameraNo].text, Point(10.0, 75.0),
			CV_FONT_HERSHEY_SIMPLEX, 2.5, CV_RGB(0.0, 0.0, 127.0), 2);
	}

	char outputName[15];
	sprintf(outputName, "Dart Cam No. %d", cameraNo);
	imshow(outputName, output[cameraNo]);
}

double DartDetector::calculateDartCoordinate(int cameraNo)
{
	// get biggest contour

	int maxContourIndex = 0;
	int maxsize = dartcontour[cameraNo].contours.
		at(maxContourIndex).size();
	for(int i=0; i < dartcontour[cameraNo].contours.size(); i++)
	{
		int maxsize = 0;
		vector<Point> cont = dartcontour[cameraNo].contours.at(i);
		if(maxsize < cont.size())
		{
			maxContourIndex = i;
			maxsize = cont.size();
		}
	}

	vector<Point> contour = dartcontour[cameraNo].contours.at(maxContourIndex);

	// get lowest point
	int biggestYPnt = 0;
	int biggestY = 0;
	for(int i=0; i<contour.size(); i++)
	{
		if(contour.at(i).y < biggestY)
			continue;
		biggestYPnt = i;
		biggestY = contour.at(i).y;
	}

	return ((double)contour.at(biggestYPnt).x - (double)dartboard[cameraNo].board.x)
		/ (double)dartboard[cameraNo].board.width * 2.0 - 1.0 ;
}

void DartDetector::calculateDartScore()
{
	initializeDartboard(0, background[0]);
	initializeDartboard(1, background[1]);

	double x = calculateDartCoordinate(0);
	double y = -calculateDartCoordinate(1);
	double alpha = PI/4.0;
	//lastScore.pnt.x = cos(alpha)*x - sin(alpha)*y;
	//lastScore.pnt.y = sin(alpha)*x + cos(alpha)*y;
	//lastScore.pnt.y *= -1.0;
	lastScore.pnt.x = x;
	lastScore.pnt.y = y;
	lastScore.isSet = true;

	double scale = 450.0 / 2.0;
	Point2d pnt(lastScore.pnt * scale);

	double length = sqrt(pnt.x*pnt.x + pnt.y*pnt.y);

	if(length > radii[5]) { lastScore.score = 0; return; }
	if(length <= radii[0]) { lastScore.score = 50; return; }
	if(length <= radii[1]) { lastScore.score = 25; return; }

	x = -pnt.y;
	y = pnt.x;

	alpha = asin(y/length);
	if(x < 0)
		alpha = PI - alpha;
	else if(y < 0)
		alpha = 2*PI + alpha;

	for(int i = 0; i < 19; i++)
	{
		double beta = 2*PI/20*(i+1) + 2*PI/40;
		if(alpha < beta) { lastScore.score = scores[i]; break; }
	}
	if(alpha < 2*PI/40 || alpha >= 2*PI/20*19+2*PI/40) { lastScore.score = scores[19]; }

	if(length >= radii[2] && length <= radii[3]) { lastScore.score *= 3; return; }
	if(length >= radii[4]) { lastScore.score *= 2; return; }

}

void DartDetector::renderVisualization()
{
	int width = 500, height = 500;
	Mat screen(width, height, CV_32FC1);
	Point2d center(width/2, height/2);

	screen = 0.0;

	cvtColor(screen, screen, CV_GRAY2RGB);

	// draw circles
	circle(screen, center, 450/2, CV_RGB(0.0, 0.0, 0.0), 2);
	
	for(int i=0; i<6; i++)
		circle(screen, center, radii[i], CV_RGB(0.0, 0.0, 1.0), 1);

	// draw borders
	for(int i=0; i<20; i++)
	{
		double alpha = 2.0*PI / 20.0 * (i+0.5);
		cv::Point2d pnt(cos(alpha), sin(alpha));
		line(screen, center + pnt*radii[1], center + pnt*170.0, CV_RGB(0.0, 0.0, 1.0));
	}

	if(lastScore.isSet)
	{
		double scale = 450.0/2.0;
		//circle(screen, center + lastScore.pnt, 5, CV_RGB(0.0, 1.0, 0.0), 2);
		line(screen, center + lastScore.pnt * scale + Point2d(0.0, -5.0),
			center + lastScore.pnt * scale + Point2d(0.0, 5.0), CV_RGB(0.0, 1.0, 0.0), 2);
		line(screen, center + lastScore.pnt * scale + Point2d(-5.0, 0.0),
			center + lastScore.pnt * scale + Point2d(5.0, 0.0), CV_RGB(0.0, 1.0, 0.0), 2);

		char pntText[3];
		sprintf(pntText, "%2d", lastScore.score);
		putText(screen, pntText, Point(10.0, 75.0),
			CV_FONT_HERSHEY_SIMPLEX, 2.5, CV_RGB(0.0, 0.0, 127.0), 2);
	}

	imshow("Visualization", screen);
}
