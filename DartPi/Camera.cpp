#include "Camera.h"

// Constructor
Camera::Camera(char *fn1, char *fn2)
{
	cam[0] = VideoCapture(0);
	cam[1] = VideoCapture(1);

#ifdef CAMERA_WIDTH
	cam[0].set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
#endif
#ifdef CAMERA_HEIGHT
	cam[0].set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
#endif
#ifdef CAMERA_WIDTH
	cam[1].set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
#endif
#ifdef CAMERA_HEIGHT
	cam[1].set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
#endif
#ifdef CAMERA_FPS
	cam[0].set(CV_CAP_PROP_FPS, CAMERA_FPS);
	cam[1].set(CV_CAP_PROP_FPS, CAMERA_FPS);
#endif

	edgeFilterLR = Mat(1, 2, CV_64F);
	edgeFilterRL = Mat(1, 2, CV_64F);

	edgeFilterLR.at<double>(0,0) = 1.;
	edgeFilterLR.at<double>(0,1) = -1.;

	edgeFilterRL.at<double>(0,0) = -1.;
	edgeFilterRL.at<double>(0,1) = 1.;
}

// Destructor
Camera::~Camera(void)
{
	cam[0].release();
	cam[1].release();
}

bool Camera::checkCameras()
{
	if(!cam[0].isOpened() || !cam[1].isOpened())
		return false;
	return true;
}

void Camera::startSequence()
{
	while(1)
	{
		cam[0] >> frame[0];
		cam[1] >> frame[1];
		imshow("cam[0]", frame[0]);
		imshow("cam[1]", frame[1]);

		printf("Frame ...\n");

		if(waitKey(1) >= 0) return;
	}


	int j = 0;

	for(;;)
	{
		cam[0] >> frame[0];
		cam[1] >> frame[1];

		for(int i = 0; i < 2; i++)
		{

			if(j % 100 == 0)
			{
				cvtColor(frame[i], startFrame[i], CV_BGR2GRAY);
				j=0;
			}

			cvtColor(frame[i], tmpFrame[i], CV_RGB2GRAY);
			equalizeHist(tmpFrame[i], tmpFrame[i]);

			GaussianBlur(tmpFrame[i], edgeFrame[i], Size(7,7), 2.5, 2.5);

			imshow("-", tmpFrame[i]);

			filter2D(tmpFrame[i], edgeFrame[i], -1, edgeFilterLR);
			edgeFrame[i] *= 5.0;

			filter2D(tmpFrame[i], startFrame[i], -1, edgeFilterRL);
			startFrame[i] *= 5.0;

			edgeFrame[i] += startFrame[i];

			threshold(edgeFrame[i], edgeFrame[i], 230, 255, CV_THRESH_BINARY);
			threshold(tmpFrame[i], tmpFrame[i], 40, 255, THRESH_BINARY_INV);

			erode(tmpFrame[i], tmpFrame[i], edgeFilterLR);
			dilate(edgeFrame[i]+tmpFrame[i], tmpFrame[i], edgeFilterLR);

			j++;
		}
		imshow("= 1. Frame", tmpFrame[0]);
		imshow("= 2. Frame", tmpFrame[1]);

		if(waitKey(1) >= 0) break;
	}
}

void Camera::showNextFrame(int i)
{
	cam[i] >> frame[i];
	imshow("Frame", frame[i]);
}

// GETTER + SETTER
VideoCapture Camera::getCamera(int i)
{
	return cam[i];
}

Mat Camera::getNextFrame(int i)
{
	Mat frame;
	cam[i] >> frame;
	cvtColor(frame, frame, CV_BGR2GRAY);
	//equalizeHist(frame, frame);
	//GaussianBlur(frame, frame, Size(2,2), 1);
	return frame;
}
