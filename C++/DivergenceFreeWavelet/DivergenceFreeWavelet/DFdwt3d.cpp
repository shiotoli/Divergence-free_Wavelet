#include "DFdwt3D.h"
void DFdwt3D::loadData(string u_path, string v_path, string w_path, string filter0_path, string filter1_path)
{
	FILE* fp = fopen(u_path.c_str(), "r");
	for (int i = 0; i<DIM; i++)
	for (int j = 0; j<DIM; j++)
	for (int k = 0; k < DIM;k++)
		fscanf(fp, "%lf", &velocityField.u[i][j][k]);
	fclose(fp);
	fp = fopen(v_path.c_str(), "r");
	for (int i = 0; i<DIM; i++)
	for (int j = 0; j<DIM; j++)
	for (int k = 0; k < DIM; k++)
		fscanf(fp, "%lf", &velocityField.v[i][j][k]);
	fclose(fp);
	fp = fopen(w_path.c_str(), "r");
	for (int i = 0; i<DIM; i++)
	for (int j = 0; j<DIM; j++)
	for (int k = 0; k < DIM; k++)
		fscanf(fp, "%lf", &velocityField.w[i][j][k]);
	fclose(fp);
	filter0.loadFilter(filter0_path);
	filter1.loadFilter(filter1_path);
}
void DFdwt3D::wavedec3d(int level)
{
	double** lastu, **lastv;
	lastu = velocityField.u;
	lastv = velocityField.v;
	int LLsize = DIM;
	for (int i = 0; i<level; i++)
	{
		double** ull, **vll, **uhl, **vhl, **ulh, **vlh, **vhh, **uhh;
		fwt2d_uv(lastu, lastv, ull, uhl, ulh, uhh, vll, vlh, vhl, vhh, LLsize);
		waveletCoeULL.push_back(ull);
		waveletCoeULH.push_back(ulh);
		waveletCoeUHL.push_back(uhl);
		waveletCoeUHH.push_back(uhh);
		waveletCoeVLL.push_back(vll);
		waveletCoeVLH.push_back(vlh);
		waveletCoeVHL.push_back(vhl);
		waveletCoeVHH.push_back(vhh);
		waveletCoeSize.push_back(LLsize / 2);
		LLsize /= 2;
		lastu = ull;
		lastv = vll;
		char name[100];
		set_field(ulh, LLsize, LLsize, 0.00);
		set_field(uhl, LLsize, LLsize, 0.00);
		set_field(uhh, LLsize, LLsize, 0.00);
		set_field(vlh, LLsize, LLsize, 0.00);
		set_field(vhl, LLsize, LLsize, 0.00);
		set_field(vhh, LLsize, LLsize, 0.00);
		//cout<<calcDivergence_biggest(ull,vll,LLsize,LLsize)<<endl;
		sprintf(name, "ull%04d.txt", i);
		print2d(string(name), ull, LLsize, LLsize);
		sprintf(name, "uhl%04d.txt", i);
		print2d(string(name), uhl, LLsize, LLsize);
		sprintf(name, "ulh%04d.txt", i);
		print2d(string(name), ulh, LLsize, LLsize);
		sprintf(name, "uhh%04d.txt", i);
		print2d(string(name), uhh, LLsize, LLsize);
		sprintf(name, "vll%04d.txt", i);
		print2d(string(name), vll, LLsize, LLsize);
		sprintf(name, "vhl%04d.txt", i);
		print2d(string(name), vhl, LLsize, LLsize);
		sprintf(name, "vlh%04d.txt", i);
		print2d(string(name), vlh, LLsize, LLsize);
		sprintf(name, "vhh%04d.txt", i);
		print2d(string(name), vhh, LLsize, LLsize);
		if (i == level - 1)
		{
			uhh[0][0] = vhh[0][0] = 1;;
			set_field(ull, vll, LLsize, LLsize,
				//uhh[0][0] = 0.3;
				//vhh[0][0] = 0.0;
				//mul(uhh,vhh,LLsize,LLsize,1);
				//mul(uhl,vhl,LLsize,LLsize,1);
				//mul(ulh,vlh,LLsize,LLsize,1);
				//uhh[3][3] = 0.3;
				//vhh[0][0] = 0.0;
				//uhh[2][2] = 5;
				//set_field_rand(uhl,vhl,LLsize,LLsize);
				//set_field(ulh,vlh,LLsize,LLsize,0.05,0.00);
				//set_field(uhl,vhl,LLsize,LLsize,0.06,0.00);
		}
	}
}
void DFdwt3D::waverec3d(int level)
{
	double** lastu = waveletCoeULL[level], **lastv = waveletCoeVLL[level], **nextu, **nextv;
	char name[100];
	for (int i = level; i >= 0; i--)
	{
		ifwt2d_uv(lastu, waveletCoeUHL[i], waveletCoeULH[i], waveletCoeUHH[i], lastv, waveletCoeVLH[i], waveletCoeVHL[i], waveletCoeVHH[i], nextu, nextv, waveletCoeSize[i]);
		recWaveletCoeULL.push_back(nextu);
		recWaveletCoeVLL.push_back(nextv);
		lastu = nextu;
		lastv = nextv;
		recWaveletCoeSize.push_back(waveletCoeSize[i] * 2);
		sprintf(name, "nextu%04d.txt", i);
		print2d(string(name), nextu, waveletCoeSize[i] * 2, waveletCoeSize[i] * 2);
		sprintf(name, "nextv%04d.txt", i);
		print2d(string(name), nextv, waveletCoeSize[i] * 2, waveletCoeSize[i] * 2);
	}
	//return ;
	cout << calcDivergence_sum(lastu, lastv, DIM, DIM) << ' ' << calcDivergence_biggest(lastu, lastv, DIM, DIM) << endl;
	print2d("..\\..\\..\\mathematica\\lastu.txt", lastu, waveletCoeSize[0] * 2, waveletCoeSize[0] * 2);
	//set_field(lastu,lastv,waveletCoeSize[0]*2,waveletCoeSize[0]*2,1,0);
	print2d("..\\..\\..\\mathematica\\lastv.txt", lastv, waveletCoeSize[0] * 2, waveletCoeSize[0] * 2);
	system("pause");

	//print2d("realu.txt",waveletCoeULL[level-1],waveletCoeSize[level]*2,waveletCoeSize[level]*2);
	//print2d("realv.txt",waveletCoeVLL[level-1],waveletCoeSize[level]*2,waveletCoeSize[level]*2);
}
void DFdwt3D::fwt3d_uvw(double*** u, double*** v, double *** w, int LLsize)
{
	double*** ullltmp, *** ullhtmp, *** ulhltmp, *** ulhhtmp, *** uhlltmp, *** uhlhtmp, *** uhhltmp, *** uhhhtmp;
	double*** vllltmp, *** vllhtmp, *** vlhltmp, *** vlhhtmp, *** vhlltmp, *** vhlhtmp, *** vhhltmp, *** vhhhtmp;
	double*** wllltmp, *** wllhtmp, *** wlhltmp, *** wlhhtmp, *** whlltmp, *** whlhtmp, *** whhltmp, *** whhhtmp;
	double*** ulll, *** ullh, *** ulhl, *** ulhh, *** uhll, *** uhlh, *** uhhl, *** uhhh;
	double*** vlll, *** vllh, *** vlhl, *** vlhh, *** vhll, *** vhlh, *** vhhl, *** vhhh;
	double*** wlll, *** wllh, *** wlhl, *** wlhh, *** whll, *** whlh, *** whhl, *** whhh;
	fwt3d(u, ullltmp, ullhtmp, ulhltmp, ulhhtmp, uhlltmp, uhlhtmp, uhhltmp, uhhhtmp, filter1.lod, filter0.lod, filter0.lod, filter1.hid, filter0.hid, filter0.hid, LLsize);
	fwt3d(v, vllltmp, vllhtmp, vlhltmp, vlhhtmp, vhlltmp, vhlhtmp, vhhltmp, vhhhtmp, filter0.lod, filter1.lod, filter0.lod, filter0.hid, filter1.hid, filter0.hid, LLsize);
	fwt3d(w, wllltmp, wllhtmp, wlhltmp, wlhhtmp, whlltmp, whlhtmp, whhltmp, whhhtmp, filter0.lod, filter0.lod, filter1.lod, filter0.hid, filter0.hid, filter1.hid, LLsize);
	new3D(ulll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(ullh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(ulhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(ulhh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(uhll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(uhlh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(uhhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(uhhh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vlll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vllh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vlhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vlhh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vhll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vhlh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vhhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vhhh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(wlll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(wllh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(wlhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(wlhh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(whll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(whlh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(whhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(whhh, LLsize / 2, LLsize / 2, LLsize / 2);
	for (int i = 0; i < LLsize / 2;i++)
	for (int j = 0; j < LLsize / 2;j++)
	for (int k = 0; k < LLsize / 2; k++)
	{
		ulll[i][j][k] = ullltmp[i][j][k];
		vlll[i][j][k] = vllltmp[i][j][k];
		wlll[i][j][k] = wllltmp[i][j][k];
		uhll[i][j][k] = vhlltmp[i][j][k];
		vhll[i][j][k] = whlltmp[i][j][k];
		whll[i][j][k] = uhlltmp[i][j][k];
		if (j != 0)
			whll[i][j][k] += 0.25*vhlltmp[i][j][k] - 0.25*vhlltmp[i][j - 1][k];
		else
			whll[i][j][k] += 0.25*vhlltmp[i][j][k] - 0.25*(scene == LOOP ? vhlltmp[i][LLsize / 2 - 1][k] : 0);
		if (k != 0)
			whll[i][j][k] += 0.25*whlltmp[i][j][k] - 0.25*whlltmp[i][j][k - 1];
		else
			whll[i][j][k] += 0.25*whlltmp[i][j][k] - 0.25*(scene == LOOP ? vhlltmp[i][j][LLsize / 2 - 1] : 0);
		ulhl[i][j][k] = ulhltmp[i][j][k];
		vlhl[i][j][k] = wlhltmp[i][j][k];
		wlhl[i][j][k] = vlhltmp[i][j][k];
		if (i!=0)
			wlhl[i][j][k]+=0.25*(vlhl[i][j][k] - vlhl[i - 1][j][k]);
		else 
			wlhl[i][j][k] += 0.25*ulhl[i][j][k] - 0.25*(scene == LOOP ? ulhltmp[LLsize / 2 - 1][j][k] : 0);
	}
}
void DFdwt3D::fwt3d_uv(double*** u, double*** &ulll, double*** &ullh, double*** &ulhl, double*** &ulhh, double*** &uhll, double*** &uhlh, double*** &uhhl, double*** &uhh, int LLsize)
{
	double*** ulltmp, ***ulhtmp, ***uhltmp, ***uhhtmp, ***vlltmp, ***vhltmp, ***vlhtmp, ***vhhtmp;
	fwt2d(u, ulltmp, uhltmp, ulhtmp, uhhtmp, filter1.lod, filter0.lod, filter1.hid, filter0.hid, LLsize);
	fwt2d(v, vlltmp, vhltmp, vlhtmp, vhhtmp, filter0.lod, filter1.lod, filter0.hid, filter1.hid, LLsize);
	new2D(ull, LLsize / 2, LLsize / 2);
	new2D(vll, LLsize / 2, LLsize / 2);
	new2D(uhl, LLsize / 2, LLsize / 2);
	new2D(vhl, LLsize / 2, LLsize / 2);
	new2D(ulh, LLsize / 2, LLsize / 2);
	new2D(vlh, LLsize / 2, LLsize / 2);
	new2D(uhh, LLsize / 2, LLsize / 2);
	new2D(vhh, LLsize / 2, LLsize / 2);
	// 	print2d("t1.txt",vlltmp,LLsize/2,LLsize/2);
	// 	print2d("t2.txt",vlhtmp,LLsize/2,LLsize/2);
	// 	print2d("t3.txt",vhltmp,LLsize/2,LLsize/2);
	// 	print2d("t4.txt",vhhtmp,LLsize/2,LLsize/2);
	for (int i = 0; i<LLsize / 2; i++)
	for (int j = 0; j<LLsize / 2; j++)
	{
		ull[i][j] = ulltmp[i][j];
		vll[i][j] = vlltmp[i][j];
		//Psi(0,1)
		uhl[i][j] = uhltmp[i][j];
		if (j != 0)
			vhl[i][j] = vhltmp[i][j] + 0.25*uhltmp[i][j] - 0.25*uhltmp[i][j - 1];
		else
			vhl[i][j] = vhltmp[i][j] + 0.25*uhltmp[i][j] - 0.25*(scene == LOOP ? uhltmp[i][LLsize / 2 - 1] : 0);
		//Psi(1,0)
		ulh[i][j] = vlhtmp[i][j];
		if (i != 0)
			vlh[i][j] = ulhtmp[i][j] + 0.25*vlhtmp[i][j] - 0.25*vlhtmp[i - 1][j];
		else
			vlh[i][j] = ulhtmp[i][j] + 0.25*vlhtmp[i][j] - 0.25*(scene == LOOP ? vlhtmp[LLsize / 2 - 1][j] : 0);
		//Psi(1,1)
		uhh[i][j] = 0.5*uhhtmp[i][j] - 0.5*vhhtmp[i][j];
		vhh[i][j] = 0.5*uhhtmp[i][j] + 0.5*vhhtmp[i][j];
	}
	release2D(ulltmp, LLsize / 2);
	release2D(vlltmp, LLsize / 2);
	release2D(uhltmp, LLsize / 2);
	release2D(vhltmp, LLsize / 2);
	release2D(ulhtmp, LLsize / 2);
	release2D(vlhtmp, LLsize / 2);
	release2D(uhhtmp, LLsize / 2);
	release2D(vhhtmp, LLsize / 2);
}
void DFdwt3D::ifwt3d_uv(double** ull, double** uhl, double** ulh, double** uhh, double** vll, double** vlh, double** vhl, double** vhh, double** &u, double** &v, int LLsize)
{
	double** ulltmp, **ulhtmp, **uhltmp, **uhhtmp, **vlltmp, **vhltmp, **vlhtmp, **vhhtmp;
	new2D(ulltmp, LLsize, LLsize);
	new2D(vlltmp, LLsize, LLsize);
	new2D(uhltmp, LLsize, LLsize);
	new2D(vhltmp, LLsize, LLsize);
	new2D(ulhtmp, LLsize, LLsize);
	new2D(vlhtmp, LLsize, LLsize);
	new2D(uhhtmp, LLsize, LLsize);
	new2D(vhhtmp, LLsize, LLsize);
	new2D(u, LLsize * 2, LLsize * 2);
	new2D(v, LLsize * 2, LLsize * 2);
	for (int i = 0; i<LLsize; i++)
	for (int j = 0; j<LLsize; j++)
	{
		ulltmp[i][j] = ull[i][j];
		vlltmp[i][j] = vll[i][j];
		//Psi(0,1)
		uhltmp[i][j] = uhl[i][j];
		//Psi(1,0)
		vlhtmp[i][j] = ulh[i][j];
		//Psi(1,1)
		uhhtmp[i][j] = uhh[i][j] + vhh[i][j];
		vhhtmp[i][j] = vhh[i][j] - uhh[i][j];
	}
	for (int i = 0; i<LLsize; i++)
	for (int j = 0; j<LLsize; j++)
	{
		if (j != 0)
			vhltmp[i][j] = vhl[i][j] - 0.25*uhltmp[i][j] + 0.25*uhltmp[i][j - 1];
		else
			vhltmp[i][j] = vhl[i][j] - 0.25*uhltmp[i][j] + 0.25*(scene == LOOP ? uhltmp[i][LLsize - 1] : 0);
		if (i != 0)
			ulhtmp[i][j] = vlh[i][j] - 0.25*vlhtmp[i][j] + 0.25*vlhtmp[i - 1][j];
		else
			ulhtmp[i][j] = vlh[i][j] - 0.25*vlhtmp[i][j] + 0.25*(scene == LOOP ? vlhtmp[LLsize - 1][j] : 0);

	}
	// 		print2d("s1.txt",vlltmp,LLsize,LLsize);
	// 		print2d("s2.txt",vlhtmp,LLsize,LLsize);
	// 		print2d("s3.txt",vhltmp,LLsize,LLsize);
	// 		print2d("s4.txt",vhhtmp,LLsize,LLsize);
	ifwt2d(ulltmp, uhltmp, ulhtmp, uhhtmp, filter1.lor, filter0.lor, filter1.hir, filter0.hir, u, LLsize);
	ifwt2d(vlltmp, vhltmp, vlhtmp, vhhtmp, filter0.lor, filter1.lor, filter0.hir, filter1.hir, v, LLsize);
	release2D(ulltmp, LLsize);
	release2D(vlltmp, LLsize);
	release2D(uhltmp, LLsize);
	release2D(vhltmp, LLsize);
	release2D(ulhtmp, LLsize);
	release2D(vlhtmp, LLsize);
	release2D(uhhtmp, LLsize);
	release2D(vhhtmp, LLsize);
}
void DFdwt3D::fwt3d(double*** LLL, double*** &lll, double*** &llh, double*** &lhl, double*** &lhh, double*** &hll, double*** &hlh, double*** &hhl, double*** &hhh, SubFilter lodx, SubFilter lody, SubFilter lodz, SubFilter hidx, SubFilter hidy, SubFilter hidz, int LLsize)
{
	double*** tmpL, ***tmpH,***tmpHL,***tmpLH,***tmpLL,***tmpHH;
	//print2d("LL2.txt", LL, LLsize, LLsize);
	conv_down3d(LLL, tmpL, 0, lodx, LLsize, LLsize, LLsize);
	conv_down3d(tmpL, tmpLL, 1, lody, LLsize / 2, LLsize, LLsize);
	conv_down3d(tmpL, tmpLH, 1, hidy, LLsize / 2, LLsize, LLsize);
	conv_down3d(tmpLL, lll, 2, lodz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpLL, llh, 2, hidz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpLH, lhl, 2, lodz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpLH, lhh, 2, hidz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(LLL, tmpH, 0, hidx, LLsize, LLsize, LLsize);
	conv_down3d(tmpH, tmpHL, 1, lody, LLsize / 2, LLsize, LLsize);
	conv_down3d(tmpH, tmpHH, 1, hidy, LLsize / 2, LLsize, LLsize);
	conv_down3d(tmpHL, hll, 2, lodz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpHL, hlh, 2, hidz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpHH, hhl, 2, lodz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpHH, hhh, 2, hidz, LLsize / 2, LLsize / 2, LLsize);
	release3D(tmpL, LLsize / 2, LLsize);
	release3D(tmpH, LLsize / 2, LLsize);
	release3D(tmpLH, LLsize / 2, LLsize/2);
	release3D(tmpHL, LLsize / 2, LLsize/2);
	release3D(tmpLL, LLsize / 2, LLsize/2);
	release3D(tmpHH, LLsize / 2, LLsize/2);
}
void DFdwt3D::ifwt3d(double*** lll, double*** llh, double*** lhl, double*** lhh, double*** hll, double*** hlh, double*** hhl, double*** hhh, SubFilter lorx, SubFilter lory, SubFilter lorz, SubFilter hirx, SubFilter hiry, SubFilter hirz, double*** &LLL, int LLsize)
{
	double*** tmplll, ***tmpllh, ***tmplhl, ***tmplhh, ***tmphll, ***tmphlh, ***tmphhl, ***tmphhh;
	double*** tmpll, ***tmphl, ***tmplh, ***tmphh,***tmpl,***tmph;

	conv_up3d(lll, tmplll, 2, lorz, LLsize, LLsize, LLsize);
	conv_up3d(llh, tmpllh, 2, hirz, LLsize, LLsize, LLsize);
	addMatrix_3d(tmplll, tmpllh, tmplll, LLsize, LLsize, LLsize * 2);

	conv_up3d(lhl, tmplhl, 2, lorz, LLsize, LLsize, LLsize);
	conv_up3d(lhh, tmplhh, 2, hirz, LLsize, LLsize, LLsize);
	addMatrix_3d(tmplhl, tmplhh, tmplhl, LLsize, LLsize, LLsize * 2);

	conv_up3d(lll, tmphll, 2, lorz, LLsize, LLsize, LLsize);
	conv_up3d(llh, tmphlh, 2, hirz, LLsize, LLsize, LLsize);
	addMatrix_3d(tmphll, tmphlh, tmplll, LLsize, LLsize, LLsize * 2);

	conv_up3d(hhl, tmphhl, 2, lorz, LLsize, LLsize, LLsize);
	conv_up3d(hhh, tmphhh, 2, hirz, LLsize, LLsize, LLsize);
	addMatrix_3d(tmphhl, tmphhh, tmphhl, LLsize, LLsize, LLsize * 2);

	conv_up3d(tmphll, tmphl, 1, lory, LLsize, LLsize, LLsize * 2);
	conv_up3d(tmphhl, tmphh, 1, hiry, LLsize, LLsize, LLsize * 2);
	addMatrix_3d(tmphl, tmphh, tmphl, LLsize, LLsize * 2, LLsize * 2);

	conv_up3d(tmplll, tmpll, 1, lory, LLsize, LLsize, LLsize * 2);
	conv_up3d(tmplhl, tmplh, 1, hiry, LLsize, LLsize, LLsize * 2);
	addMatrix_3d(tmpll, tmplh, tmpll , LLsize, LLsize * 2, LLsize * 2);

	conv_up3d(tmpll, LLL, 0, lorx, LLsize, LLsize * 2, LLsize * 2);
	conv_up3d(tmphl, tmph, 0, hirx, LLsize, LLsize * 2, LLsize * 2);
	addMatrix_3d(LLL, tmph, LLL, LLsize*2, LLsize * 2, LLsize * 2);

	release3D(tmph, LLsize * 2, LLsize * 2);
	release3D(tmpll, LLsize, LLsize * 2);
	release3D(tmphl, LLsize, LLsize * 2);
	release3D(tmplh, LLsize, LLsize * 2);
	release3D(tmphh, LLsize, LLsize * 2);
	release3D(tmphhh, LLsize, LLsize);
	release3D(tmphhl, LLsize, LLsize);
	release3D(tmphlh, LLsize, LLsize);
	release3D(tmphll, LLsize, LLsize);
	release3D(tmplll, LLsize, LLsize);
	release3D(tmpllh, LLsize, LLsize);
	release3D(tmplhl, LLsize, LLsize);
	release3D(tmplhh, LLsize, LLsize);
}
void DFdwt3D::conv_down3d(double*** input, double*** &output, int dir, SubFilter filter, int input_xsize, int input_ysize, int input_zsize)
{
	int filterSize = filter.filterLen;
	if (dir == 0)
	{
		double*** outtmp;// = new double**[input_xsize];
		new3D(outtmp, input_xsize / 2, input_ysize, input_zsize);
		reset3D(outtmp, input_xsize / 2, input_ysize, input_zsize);
		for (int i = 0; i<input_xsize/2; i++)
		{
			for (int j = 0; j<input_ysize; j++)
			{
				for (int k = 0; k < input_zsize;k++)
				for (int l = 0; l<filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = i * 2 + 1 +/*(filterSize-1)+*/l + filter.startid;
#else
					int input_id = i * 2 + 1 -/*(filterSize-1)+*/l - filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + 10 * input_xsize) % input_xsize;
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_xsize)
					{
						if (scene == LOOP)
							input_id %= input_xsize;
						else if (scene == BOUNDARY)
							continue;
					}
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[input_id][j][k];
				}
			}
		}
		output = outtmp;
	}
	else if (dir == 1)
	{
		double*** outtmp;// = new double**[input_xsize];
		new3D(outtmp, input_xsize, input_ysize/2, input_zsize);
		reset3D(outtmp, input_xsize, input_ysize/2, input_zsize);
		for (int i = 0; i<input_xsize; i++)
		{
			for (int j = 0; j<input_ysize / 2; j++)
			{
				for (int k = 0; k < input_zsize;k++)
				for (int l = 0; l < filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = j * 2 + 1 +/*(filterSize-1)+*/l + filter.startid;
#else
					int input_id = j * 2 + 1 -/*(filterSize-1)+*/l - filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + 10 * input_ysize) % input_ysize;
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_ysize)
					{
						if (scene == LOOP)
							input_id %= input_ysize;
						else if (scene == BOUNDARY)
							continue;
					}
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[i][input_id][k];
				}
			}
		}
		output = outtmp;
	}
	else if (dir == 2)
	{
		double*** outtmp;// = new double**[input_xsize];
		new3D(outtmp, input_xsize, input_ysize, input_zsize / 2);
		reset3D(outtmp, input_xsize, input_ysize, input_zsize / 2);
		for (int i = 0; i<input_xsize; i++)
		{
			for (int j = 0; j<input_ysize; j++)
			{
				for (int k = 0; k < input_zsize / 2;k++)
				for (int l = 0; l<filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = k * 2 + 1 +/*(filterSize-1)+*/l + filter.startid;
#else
					int input_id = k * 2 + 1 -/*(filterSize-1)+*/l - filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + 10 * input_zsize) % input_zsize;
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_zsize)
					{
						if (scene == LOOP)
							input_id %= input_zsize;
						else if (scene == BOUNDARY)
							continue;
					}
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[i][j][input_id];
				}
			}
		}
		output = outtmp;

	}
}
void DFdwt3D::conv_up3d(double*** input, double*** &output, int dir, SubFilter filter, int input_xsize, int input_ysize, int input_zsize)
{
	int filterSize = filter.filterLen;
	if (dir == 0)
	{
		double*** outtmp;// = new double*[input_ysize];
		new3D(outtmp, input_xsize * 2, input_ysize, input_zsize);
		reset3D(outtmp, input_xsize * 2, input_ysize, input_zsize);
		for (int i = 0; i<input_xsize*2; i++)
		{
			for (int j = 0; j<input_ysize; j++)
			{
				for (int k = 0; k < input_zsize;k++)
				for (int l = 0; l<filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = i -/*(filterSize-1)+*/l - filter.startid;
#else
					int input_id = i +/*(filterSize-1)+*/l + filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + input_xsize * 20) % (input_xsize * 2);
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_xsize * 2)
					{
						if (scene == LOOP)
							input_id %= input_xsize * 2;
						else if (scene == BOUNDARY)
							continue;
					}
					if (input_id % 2 == 0)
						continue;
					input_id /= 2;
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[input_id][j][k];
				}
			}
		}
		output = outtmp;
	}
	else if (dir == 1)
	{
		double*** outtmp;// = new double*[input_ysize];
		new3D(outtmp, input_xsize, input_ysize*2, input_zsize);
		reset3D(outtmp, input_xsize, input_ysize*2, input_zsize);
		for (int i = 0; i<input_xsize; i++)
		{
			for (int j = 0; j<input_ysize * 2; j++)
			{
				for (int k = 0; k < input_zsize;k++)
				for (int l = 0; l<filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = j -/*(filterSize-1)+*/l - filter.startid;
#else
					int input_id = j +/*(filterSize-1)+*/l + filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + input_ysize * 20) % (input_ysize * 2);
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_ysize * 2)
					{
						if (scene == LOOP)
							input_id %= input_ysize * 2;
						else if (scene == BOUNDARY)
							continue;
					}
					if (input_id % 2 == 0)
						continue;
					input_id /= 2;
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[i][input_id][k];
				}
			}
		}
		output = outtmp;
	}
	else if (dir == 2)
	{
		double*** outtmp;// = new double*[input_ysize];
		new3D(outtmp, input_xsize, input_ysize, input_zsize*2);
		reset3D(outtmp, input_xsize, input_ysize, input_zsize*2);
		for (int i = 0; i<input_xsize; i++)
		{
			for (int j = 0; j<input_ysize; j++)
			{
				for (int k = 0; k < input_zsize*2; k++)
				for (int l = 0; l<filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = k -/*(filterSize-1)+*/l - filter.startid;
#else
					int input_id = k +/*(filterSize-1)+*/l + filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + input_zsize * 20) % (input_zsize * 2);
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_zsize * 2)
					{
						if (scene == LOOP)
							input_id %= input_zsize * 2;
						else if (scene == BOUNDARY)
							continue;
					}
					if (input_id % 2 == 0)
						continue;
					input_id /= 2;
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[i][j][input_id];
				}
			}
		}
		output = outtmp;

	}
}
