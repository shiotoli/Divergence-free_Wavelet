#pragma once
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