#include "DFdwt2D.h"
#include "DFdwt3D.h"
DFdwt2D df_dwt2d;
DFdwt3D df_dwt3d;
int main()
{
	//df_dwt3d.loadData("testu.txt","testv.txt","testw.txt","cdf22.txt","cdf31.txt");
	//df_dwt3d.test();
	//return 1;
	df_dwt3d.loadData("200_w.txt","200_v.txt","200_u.txt","cdf22.txt","cdf31.txt");
	df_dwt3d.wavedec3d(5);
	df_dwt3d.waverec3d(4);
	df_dwt3d.release();
	system("pause");
	//df_dwt2d.loadData("uelocity.txt","velocity.txt","cdf22.txt","cdf31.txt");
	//df_dwt2d.wavedec2d(7);
	//df_dwt2d.waverec2d(6);
}