#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;
#define IS_VECTOR(a) ((typeof(a)==typeof(vector<int>))||(typeof(a)==typeof(vector<float>))||(typeof(a)==typeof(vector<double>))||(typeof(a)==typeof(vector<string>))||(typeof(a)==typeof(vector<bool>)))

namespace ConfigParse
{
	enum Type{INT,DOUBLE,FLOAT,BOOL,STRING,INT_VECTOR,DOUBLE_VECTOR,FLOAT_VECTOR,BOOL_VECTOR,STRING_VECTOR};
	template<typename T> struct Leaf{
			string name;
			T value;
	};
	template <typename T> struct Node{
		vector<Node<T> > sonNode;
		Node();
		int createSon(string name);
		int createLeaf(string name, T value);
		string name;
		vector<Leaf<T> > leafs;
	};
	class ConfigParse
	{
	public:
		ConfigParse();
		ConfigParse(string s);
		void parse(string s);
		template <typename TN>
		bool getValue(vector<string> s,TN& a,bool append =false);
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
		template <typename TN>
		bool insert(vector<string> path,TN a,bool append = false);
		template <typename TN>
		Node<TN>* getRoot();
	};
	template <typename T>
	Node<typename T>::Node()
	{
		sonNode.clear();
		leafs.clear();
	}
	template <typename T>
	int Node<T>::createSon(string name)
	{
		Node<T> tmp;
		tmp.name = name;
		sonNode.push_back(tmp);
		return sonNode.size()-1;
	}
	template <typename T>
	int Node<T>::createLeaf(string name,T value)
	{
		Leaf<T> tmp;
		tmp.name = name;
		tmp.value = vaule;
		leafs.push_back(tmp);
		return leafs.size()-1;
	}
	void ConfigParse::parse(string s)
	{
		FILE* fp;
		fp = fopen(s.c_str(),"r");

	}
	ConfigParse::ConfigParse()
	{

	}
	template <typename TN>
	bool ConfigParse::insert(vector<string> path,TN a,bool append)
	{
		Node<TN>* now = getRoot<TN>();
		for (int i = 0;i<path.size()-1;i++)
		{
			bool find = false;
			for (int j = 0;j<now->sonNode.size();j++)
				if (now->sonNode[j].name==path[i])
				{
					now = &(now->sonNode[j]);
					find = true;
					break;
				}
			if (!find)
			{
				for (int j = i;j<path.size()-1;j++)
				{
					now = &(now->sonNode[now->createSon(path[j])]);
				}
				now->createLeaf(path[path.size()-1],a);
				return true;
			}
		}
		bool find = false;
		for (int i = 0;i<now->leafs.size();i++)
		if (now->leafs[i].name == path[path[i].size()-1])
		{
			if (!append)
				return false;
			else
				if (IS_VECTOR(TN))
					leafs[i].value.append(a)
				else
					return false;
			return true;
		}
		now->createLeaf(path[path.size()-1],a);
		return true;
	}
	template <typename TN>
	bool ConfigParse::getValue(vector<string> path,TN& a,bool append)
	{
		Node<TN>* now = getRoot<TN>();
		for (int i = 0;i<path.size()-1;i++)
		{
			bool find = false;
			for (int j = 0;j<now->sonNode.size();j++)
				if (now->sonNode[j].name==path[i])
				{
					now = &(now->sonNode[j]);
					find = true;
					break;
				}
				if (!find)
				{
					return false;
				}
		}
		bool find = false;
		for (int i = 0;i<now->leafs.size();i++)
			if (now->leafs[i].name == path[path[i].size()-1])
			{
				if (!append)
				{
					a= now->leafs[i].value;
					return true;
				}
				if (!IS_VECTOR(a))
					return false;
				a.append(now->leafs[i].value);
				return true;
			}
		return false;
	}
	template <typename TN>
	Node<TN>* ConfigParse::getRoot()
	{
		if (typeof(TN)==typeof(int))
			return &root_int;
		if (typeof(TN)==typeof(float))
			return &root_float;
		if (typeof(TN)==typeof(double))
			return &root_double;
		if (typeof(TN)==typeof(bool))
			return &root_bool;
		if (typeof(TN)==typeof(string))
			return &root_string;
		if (typeof(TN)==typeof(vector<int>))
			return &root_vector_int;
		if (typeof(TN)==typeof(vector<double>))
			return &root_vector_double;
		if (typeof(TN)==typeof(vector<float>))
			return &root_vector_float;
		if (typeof(TN)==typeof(vector<bool>))
			return &root_vector_bool;
		if (typeof(TN)==typeof(vector<string>))
			return &root_vector_string;
	}
};