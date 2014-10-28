#pragma once
#include "stdafx.h"
#include "Vector2D.h"
#include "BSplineWavelet1D.h"
inline Vector2D BigPhi(double x,double y)
{
	return Vector2D(Phi_1(x)*(Phi_0(y)-Phi_0(y-1)),-Phi_1(y)*(Phi_0(x)-Phi_0(x-1)));
}
inline Vector2D BigPhi(Vector2D v2d)
{
	return BigPhi(v2d.x,v2d.y);
}
inline Vector2D BigPsi_10(double x,double y)
{
	return Vector2D(-0.25*Psi_1(x)*(Phi_0(y)-Phi_0(y-1)),Psi_0(x)*Phi_1(y));
}
inline Vector2D BigPsi_10(Vector2D v2d)
{
	return BigPsi_10(v2d.x,v2d.y);
}
inline Vector2D BigPsi_01(double x,double y)
{
	return Vector2D(Phi_1(x)*Psi_0(y),-0.25*(Phi_0(x)-Phi_0(x-1))*Psi_1(y));
}
inline Vector2D BigPsi_01(Vector2D v2d)
{
	return BigPsi_01(v2d.x,v2d.y);
}
inline Vector2D BigPsi_11(double x, double y)
{
	return Vector2D(Psi_1(x)*Psi_0(y),-Psi_0(x)*Psi_1(y));
}
inline Vector2D BigPsi_11(Vector2D v2d)
{
	return BigPsi_11(v2d.x,v2d.y);
}
