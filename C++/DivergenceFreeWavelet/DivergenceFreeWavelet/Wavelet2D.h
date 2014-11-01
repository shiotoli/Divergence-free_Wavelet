#include "stdafx.h"
class Wavelet2D
{
public:
	Wavelet2D();

private:
	double data[DIM][DIM];
};
Wavelet2D::Wavelet2D()
{
	for (int i = 0;i<DIM;i++)
		for (int j = 0;j<DIM;j++)
			data[i][j] = 0;
}