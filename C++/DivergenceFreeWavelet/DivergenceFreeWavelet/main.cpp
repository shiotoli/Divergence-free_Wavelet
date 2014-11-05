#include "DFdwt2D.h"
DFdwt2D df_dwt2d;
int main()
{
	df_dwt2d.loadData("testu.txt","testv.txt","cdf22.txt","cdf31.txt");
	df_dwt2d.wavedec2d(2);
	df_dwt2d.waverec2d(1);
}