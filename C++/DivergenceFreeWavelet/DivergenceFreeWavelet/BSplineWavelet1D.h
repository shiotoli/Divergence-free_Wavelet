#pragma once
#include "stdafx.h"
inline double Phi_0(double x)
{
	if (x>=-1 && x<=0) return x+1;
	if (x>=0 && x<=1) return 1-x;
	return 0;
}
inline double Psi_0(double x)
{
	return -Phi_0(2*x+1)/4.0-Phi_0(2*x)/2.0+Phi_0(2*x-1)/2.0*3-Phi_0(2*x-2)/2.0-Phi_0(2*x-3)/4.0;
}
inline double Phi_1(double x)
{
	if (x>-1 && x<=0)
		return 1/2.0+x+x*x/2.0;
	if (0<x && x<=1)
		return 1.0/2+x-x*x;
	if (1<x && x<=2)
		return 2-2*x+x*x/2.0;
	return 0;
}
inline double Psi_1(double x)
{
	return -(Phi_1(2*x+1)/2.0+Phi_1(2*x)/2.0*3-Phi_1(2*x-1)/2.0*3-Phi_1(2*x-2)/2);
}