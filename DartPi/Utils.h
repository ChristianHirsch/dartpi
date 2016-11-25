#pragma once

#define NUM_FRAMES 10

#define CAMERA_USED 0

#define DEBUG 0

#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240

#define SHOW_WINDOWS 1

enum STATUS
{
	calibrating,
	ready,
	calculating
};

enum camera_detection
{
	nothing,
	dart,
	change
};