#include "stdafx.h"
struct Vector2D
{
	2DVector(double x_,double y_):x(x_),y(y_){}
	union{
		double x,y;	
	}v[2];
};