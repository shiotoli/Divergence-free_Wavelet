#pragma once
#include <iostream>
#include <vector>
using namespace std;
namespace ConfigParse
{
	enum Type{INT,DOUBLE,FLOAT,BOOL,STRING,INT_VECTOR,DOUBLE_VECTOR,FLOAT_VECTOR,BOOL_VECTOR,STRING_VECTOR};
	template<typename T> struct Leaf{
			string name;
			T value;
	};
	template <typename T> struct Node{
		vector<Node<T> > sonNode;
		string name;
		Leaf<T> value;
	};
	class ConfigParse
	{
	public:
		ConfigParse();
		ConfigParse(string s);
		void parse(string s);
		bool getValue(string s,int& a);
		bool getValue(string s,float& a);
		bool getValue(string s,double& a);
		bool getValue(string s,string& a);
		bool getValue(string s,bool& a);
		bool getValue(string s,vector<int>& a);
		bool getValue(string s,vector<float>& a);
		bool getValue(string s,vector<double>& a);
		bool getValue(string s,vector<string>& a);
		bool getValue(string s,vector<bool>& a);
	private:
		Node<int> root_int;
		Node<float> root_float;
		Node<double> root_double;
		Node<string> root_string;
		Node<bool> root_bool;
		Node<vector<int> > root_vector_int;
		Node<vector<float> > root_vector_float;
		Node<vector<double> > root_vector_double;
		Node<vector<string> > root_vector_string;
		Node<vector<bool> > root_vector_bool;
		bool insert(string path,int a);
		bool insert(string path,double a);
		bool insert(string path,float a);
		bool insert(string path,bool a);
		bool insert(string path,string a);
		bool insert(string path,vector<int> a);
		bool insert(string path,vector<string> a);
		bool insert(string path,vector<double> a);
		bool insert(string path,vector<float> a);
	};
};
void ConfigParse::ConfigParse::parse(string s)
{
	FILE* fp;
	fp = fopen(s.c_str(),"r");
	
}
ConfigParse::ConfigParse::ConfigParse()
{

}
bool ConfigParse::ConfigParse::insert(string path,int a)
{

}