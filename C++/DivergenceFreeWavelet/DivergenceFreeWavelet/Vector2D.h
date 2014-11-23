#pragma once
#include "stdafx.h"
#ifdef MEMLEAK
#include "memleak.h"
#endif
class Vector2D
{
public:
	Vector2D(double x_,double y_):x(x_),y(y_){}
	union{
		double x,y;	
	}v[2];
};