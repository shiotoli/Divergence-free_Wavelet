#include "stdafx.h"
struct SubFilter
{
	SubFilter(){filter.clear();}
	int filterLen;
	int startid;
	vector<double> filter;
};
class Filter
{
public:
	Filter(){}
	SubFilter lod,hid,lor,hir;
	int startid;
	int filterLen;
	void loadFilter(string filter_pos);
};
void Filter::loadFilter(string filter_path)
{
	FILE* fp;
	double tmp;
	fp = fopen(filter_path.c_str(),"r");
	fscanf(fp,"%d %d",&filterLen,&startid);
	lod.startid = lor.startid = hid.startid = hir.startid = startid;
	lod.filterLen = lor.filterLen = hid.filterLen = hir.filterLen = filterLen;
	for (int j = 0;j<filterLen;j++)
	{
		fscanf(fp,"%lf",&tmp);
		lod.filter.push_back(tmp);
	}
	for (int j = 0;j<filterLen;j++)
	{
		fscanf(fp,"%lf",&tmp);
		hid.filter.push_back(tmp);
	}
	for (int j = 0;j<filterLen;j++)
	{
		fscanf(fp,"%lf",&tmp);
		lor.filter.push_back(tmp);
	}
	for (int j = 0;j<filterLen;j++)
	{
		fscanf(fp,"%lf",&tmp);
		hir.filter.push_back(tmp);
	}
	fclose(fp);
}