#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define DIM 512
#define numdims 2

int filterlen=6;
int imsize[2]={DIM,DIM};

float cdf22[]={
		0.0,-0.17677669529663689,0.35355339059327379,1.0606601717798214,0.35355339059327379,-0.17677669529663689,
		0.0,0.35355339059327379,-0.70710678118654757,0.35355339059327379,0.0,0.0,
		0.0,0.35355339059327379,0.70710678118654757,0.35355339059327379,0.0,0.0,
		0.0,0.17677669529663689,0.35355339059327379,-1.0606601717798214,0.35355339059327379,0.17677669529663689
		};
float cdf31[]={
		0.0,-0.35355339059327379,1.0606601717798214,1.0606601717798214,-0.35355339059327379,0.0 ,
		0.0,-0.17677669529663689,0.53033008588991071,-0.53033008588991071,0.17677669529663689,0.0,
		0.0,0.17677669529663689,0.53033008588991071,0.53033008588991071,0.17677669529663689,0.0,
		0.0,-0.35355339059327379,-1.0606601717798214,1.0606601717798214,0.35355339059327379,0.0
};

struct filter{
	float *hid0,*lod0,*hid1,*lod1,*hir0,*lor0,*hir1,*lor1;
	filter(){
		lod0 = cdf22; lod1 = cdf31;
		hid0 = lod0 + filterlen*1;
		hid1 = lod1 + filterlen*1;
		
		lor0 = lod0 + filterlen*2;
		lor1 = lod1 + filterlen*2;
		hir0 = lod0 + filterlen*3;
		hir1 = lod1 + filterlen*3;
	}
};

struct velocity{
	float* u,* v;
	velocity(){
		u=new float[DIM*DIM];
		v=new float[DIM*DIM];
		printf("malloc velocity\n");
	}
	~velocity(){
		delete u;
		delete v;
		printf("delete velocity\n");
	}
};

class dwt{
	public:
		velocity water;
		float* resu, *resv;
		filter thefilter;
		int* wavelet_sizes;
		void readdata(float* usrc,float* vsrc);
		void calc();
		void solve(float* resu,float* resv);
		void create_waveletsz();
		void fwt2_cpu(float* src,float* dst,int dire);
		void conv_down_2d(float* out,float* in,int size1,int skip1,int size2,int skip2,float* filter);
		dwt(){}	
		dwt(int levels);
	private:
		int numLevels;
		int numcoeff;
		int numcoarse;
};

dwt::dwt(int levels):numLevels(levels){
}

//create array
void dwt::create_waveletsz(){
	int numSubband=3; //3 for 2d
	wavelet_sizes = new int[numdims*(numLevels+2)];
	for(int d=0;d<numdims;d++){
		wavelet_sizes[d+numdims*(numLevels+1)]=imsize[d];
	}
	
	int numsubcoef;
	numcoeff=0;
	for(int l=numLevels;l>=1;--l){
		numsubcoef = 1;
		for(int d=0;d<numdims;d++){
			wavelet_sizes[d+numdims*l]=(wavelet_sizes[d+numdims*(l+1)]+filterlen-1)/2;
			numsubcoef *= wavelet_sizes[d+numdims*l];
		}
		numcoeff += numSubband*numsubcoef;
		if(l==1)
			numcoarse = numsubcoef;
	}
	numsubcoef = 1;
	for(int d=0;d<numdims;d++){
		wavelet_sizes[d]=wavelet_sizes[numdims+d];
		numsubcoef *= wavelet_sizes[d];
	}
	numcoeff+= numsubcoef;
	resu = new float[numcoeff];
	resv = new float[numcoeff];
}

void dwt::readdata(float* usrc,float *vsrc){
	printf("read data\n");
	for(int i=0;i<DIM*DIM;i++){
		water.u[i] = usrc[i];
		water.v[i] = vsrc[i];
	}
}

//calculate
void dwt::calc(){
	printf("begin calculation\n");
	fwt2_cpu(water.u,resu,0);
	fwt2_cpu(water.v,resv,1);
	solve(resu,resv);
}

void dwt::solve(float* resu,float*  resv){
	float* HxLy1 = resu + wavelet_sizes[0]*wavelet_sizes[1];
	float* HxLy2 = resv + wavelet_sizes[0]*wavelet_sizes[1];
	for(int l=1;l<=numLevels;l++){
		HxLy1 += 3 *wavelet_sizes[0+2*l] * wavelet_sizes[1+2*l];
		HxLy2 += 3 *wavelet_sizes[0+2*l] * wavelet_sizes[1+2*l];
	}
	int dx = DIM;
	int dy = DIM;
	int dxnext = wavelet_sizes[0 + 2*numLevels];
	int dynext = wavelet_sizes[0 + 2*numLevels];
	int blocksize = dxnext*dynext;
	for(int l=numLevels; l>=1; l--){
		dxnext = wavelet_sizes[0+2*l];
		dynext = wavelet_sizes[1+2*l];
		blocksize = dxnext*dynext;

		HxLy1 = HxLy1 -3*blocksize;
		HxLy2 = HxLy2 -3*blocksize;
		float* LxHy1 = LxHy1 + blocksize;
		float* HxHy1 = LxHy1 + blocksize;

		float* LxHy2 = HxLy2 + blocksize;
		float* HxHy2 = LxHy2 + blocksize;
		for(int k=0; k<dynext; k++){
			for(int i=0; i<dxnext; i++){
				int ind = i+k*dxnext;
				float wcx10 = HxLy1[ind];
				float wcy10 = HxLy2[ind];
				float wcx01 = LxHy1[ind];
				float wcy01 = LxHy2[ind];
				float wcx11 = HxHy1[ind];
				float wcy11 = HxHy2[ind];

				HxLy1[ind]=wcy10;
				LxHy1[ind]=wcx01;
				HxHy1[ind]=0.5*(wcx11-wcy11);

				HxLy2[ind]=wcx10+0.25*wcy10;
				LxHy2[ind]=wcy01+0.25*wcx01;
				HxHy2[ind]=0.5*(wcx11+wcy11);
			}
		}
		for(int k=0; k<dynext; k++){
			for(int i=0; i<dxnext; i++){
				int ind = i+k*dxnext;
				int indxs = ind-1;
				int indys = ind-dxnext;
				if(i==0)
					indxs=0;
				if(k==0)
					indys=0;

				float wcx01xs = LxHy1[indxs];
				float wcy10ys = HxLy2[indys];
				
				HxLy2[ind] -= 0.25*wcy10ys;
				LxHy2[ind] -= 0.25*wcx01xs;
			}
		}
		dx = dxnext;
		dy = dynext;
	}

}

//fwt
void dwt::fwt2_cpu(float* src,float* dst,int dire){
	
	float* HxLy = dst + wavelet_sizes[0]*wavelet_sizes[1];
	for(int l=1;l<numLevels;l++)
		HxLy += 3*wavelet_sizes[0+2*l]*wavelet_sizes[1+2*l];
	int dx = imsize[0];
	int dy = imsize[1];
	int dxnext = wavelet_sizes[0+2*numLevels];
	int dynext = wavelet_sizes[1+2*numLevels];
	int blocksize = dxnext * dynext;

//allocate some arrays
	float* LxLy = new float[blocksize];
	float* tempy = new float[dx*dynext];
	float* tempxy = new float[blocksize];
	
	float* lodx = thefilter.lod0;
	float* lody = thefilter.lod0;
	float* hidx = thefilter.hid0;
	float* hidy = thefilter.hid0;
	if(dire==0){
		lodx=thefilter.lod1;
		hidx=thefilter.hid1;
	}
	if(dire==1){
		lody=thefilter.lod1;
		hidy=thefilter.hid1;
	}
	for(int l=numLevels;l>=1;--l){
		dxnext=wavelet_sizes[0+2*l];
		dynext=wavelet_sizes[1+2*l];
		blocksize=dxnext*dynext;
		HxLy= HxLy - 3*blocksize;
		float* LxHy = HxLy+blocksize;
		float* HxHy = LxHy+blocksize;
		float* LxLy = HxHy+blocksize;
		int newdy=(dy+filterlen-1)/2;
		//Ly
		conv_down_2d(tempy,src,dy,dx,dx,1,lody);
		conv_down_2d(LxLy,tempy,dx,1,newdy,dx,lodx);
		conv_down_2d(HxLy,tempy,dx,1,newdy,dx,lodx);
		//Hy
		conv_down_2d(tempy,src,dy,dx,dx,1,hidy);
		conv_down_2d(LxHy,tempy,dx,1,newdy,dx,lodx);
		conv_down_2d(LxLy,tempy,dx,1,newdy,dx,lodx);
		memcpy(tempxy,LxLy,blocksize*sizeof(float));
		src=tempxy;
		dx=dxnext;
		dy=dynext;
	}
	memcpy(dst,src,wavelet_sizes[0]*wavelet_sizes[1]*sizeof(float));
	delete LxLy;
	delete tempy;
	delete tempxy;
}

void dwt::conv_down_2d(float* out,float* in,int size1,int skip1,int size2,int skip2,float* filter){
	int outsize1 = (size1 + filterlen -1)/2;
	int outskip2;
	if(skip2 > skip1)
		outskip2=skip2*outsize1/size1;
	else
		outskip2=skip2;
	int i2;
	for(i2=0;i2<size2;i2++){
		int i1;
		for(i1=0; i1<outsize1; i1++){
			out[i2*outskip2+i1*skip1]=0.0f;
			int k;
			for(k=0; k<filterlen; k++){
				int out_i1 = 2*i1+1-(filterlen-1)+k;
				if(out_i1<0) out_i1 = -out_i1-1;
				if(out_i1>=size1) out_i1 = size1-1-(out_i1-size1);
				out[i2*outskip2+i1*skip1] += in[i2*skip2+out_i1*skip1];
			}
		}
	}
}

int main(){
	dwt peter;
	float* usrc=NULL,*vsrc=NULL;
	peter.readdata(usrc,vsrc);
	peter.calc();
	return 0;
}



