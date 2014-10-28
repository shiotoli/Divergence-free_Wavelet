#pragma once
#include <iostream>
#include <vector>
#include <highgui.h>
#include <cv.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "libconfig.hh"
using namespace std;
using namespace cv;
#define AXIS_X 0
#define AXIS_Y 1
#define sqr(x) ((x)*(x))
#define CONFIG_SAFE_CALL(x)	if (!(x))	printf("%s return a wrong result!\n", #x);