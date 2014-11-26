#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<stdio.h>
using namespace std;
#define IS_VECTOR(a) ((typeid(a)==typeid(vector<int>))||(typeid(a)==typeid(vector<float>))||(typeid(a)==typeid(vector<double>))||(typeid(a)==typeid(vector<string>))||(typeid(a)==typeid(vector<bool>)))

#include<sstream>
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
		bool parse(string s);
		template <typename TN>
		bool getValue(vector<string> s,TN& a);
		template <typename TN>
		bool getValue(string s,TN& a);
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
		bool insert(vector<string> path,TN a);
		//void processStr(vector<string> svector,int i);
		template <typename TN>
		Node<TN>* getRoot();
	};
	inline void replace_all_bad(string& str)   
	{   
		string::size_type pos(0);
		while(true)
		{   
			if((pos=str.find('\t'))!=string::npos)   
				str.erase(pos,1);
			else break;
		}
		while(true)
		{     
			if((pos=str.find(' '))!=string::npos)   
				str.erase(pos,1);
			else break;
		}
		while(true)
		{     
			if((pos=str.find('\r'))!=string::npos)   
				str.erase(pos,1);
			else break;
		}
		while(true)
		{     
			if((pos=str.find('\n'))!=string::npos)   
				str.erase(pos,1);
			else break;
		}
		while(true)
		{     
			if((pos=str.find('='))!=string::npos)   
				str.replace(pos,1,":");
			else break;
		}
		return ;
	} 
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
		tmp.value = value;
		leafs.push_back(tmp);
		return leafs.size()-1;
	}
	template <class Type>  
	inline bool stringToNum(string str,Type& value)  
	{  
		istringstream iss(str);  
		//Type num;  
		if(iss >> value)
			return true;
		return false;
	}
	bool ConfigParse::parse(string path)
	{
		FILE* fp;
		fopen_s(&fp,path.c_str(),"r");
		string s,s1;
		vector<string> svector;
		svector.clear();
		char tmp[100];
		//split into line
		while (!feof(fp))
		{
			fscanf_s(fp,"%s",&tmp,100);
			s = tmp;
			replace_all_bad(s);
			s1 = "";
			int now = 0;
			//s = s;
			for (unsigned int i = 0;i<s.length();i++)
			{
				//string last = svector[svector.size()-1];
				if (s[i] == ';' && s1.length()!=0)
				{
					if (s1[0] ==':'||(svector.size()!=0&&svector[svector.size()-1][svector[svector.size()-1].length()-1]==':'))
					{
						svector[svector.size()-1]+=s1;
					}
					else
						svector.push_back(s1);
					s1 = "";
				}
				else if (s[i] == '{')
				{
					if (s1.length()!=0)
					{
						svector.push_back(s1);
						s1 = "";
					}
					svector.push_back("{");
				}
				else if (s[i] == '}')
				{
					if (s1.length()!=0)
					{
						if (s1[0] ==':'||(svector.size()!=0&&svector[svector.size()-1][svector[svector.size()-1].length()-1]==':'))
						{
							svector[svector.size()-1]+=s1;
						}
						else
							svector.push_back(s1);
						s1 = "";
					}
					svector.push_back("}");

				}
				else s1 += s[i];
			}
			if (s1.length()!=0)
			{
				if (s1[0] ==':'||(svector.size()!=0&&svector[svector.size()-1][svector[svector.size()-1].length()-1]==':'))
				{
					svector[svector.size()-1]+=s1;
				}
				else
					svector.push_back(s1);
				s1 = "";
			}
		}
		vector<string> list;
		list.clear();
		for (unsigned int i = 0;i<svector.size();i++)
		{
			string str = svector[i];
			if (str[str.length()-1]==':')
			{
				if (i == svector.size()-1 || svector[i+1]!="{")
					return false;
				list.push_back(str.substr(0,str.length()-1));
				i++;
				continue;
			}
			if (str=="}")
			{
				list.pop_back();
				continue;
			}
			string leftvalue,rightvalue,rv="";
			int pos;
			if((pos=str.find(':'))==string::npos)
				return false;
			leftvalue = str.substr(0,pos);
			rightvalue = str.substr(pos+1,255);
			if (rightvalue[rightvalue.length()-1] == ';')
				rightvalue.erase(rightvalue.length()-1,1);
			for (unsigned int j = 0;j<rightvalue.length();j++)
				rv += tolower(rightvalue[j]);
			if (rv == "true"||rv == "false")
			{
				list.push_back(leftvalue);
				if(!insert(list,rv=="true"?true:false))
					return false;
				list.pop_back();
				continue;
			}
			if (rightvalue[0] == '\"'&&rightvalue[rightvalue.length()-1] == '\"')
			{
				list.push_back(leftvalue);
				if(!insert(list,rightvalue.substr(1,rightvalue.length()-2)))
					return false;
				list.pop_back();
				continue;
			}
			if (rightvalue.find('e')==string::npos&&rightvalue.find('.')==string::npos)
			{
				list.push_back(leftvalue);
				int v;
				if (!stringToNum<int>(rightvalue,v))
					return false;
				if(!insert(list,v))
					return false;
				list.pop_back();
				continue;
			}
			if (rightvalue[rightvalue.length()-1] == 'f')//.find('e')==string::npos&&rightvalue.find('.')==string::npos)
			{
				list.push_back(leftvalue);
				float v;
				if (!stringToNum<float>(rightvalue,v))
					return false;
				if(!insert(list,v))
					return false;
				list.pop_back();
				continue;
			}
			list.push_back(leftvalue);
			double v;
			if (!stringToNum<double>(rightvalue,v))
				return false;
			if(!insert(list,v))
				return false;
			list.pop_back();
			continue;
		}
		return true;
	}
	ConfigParse::ConfigParse()
	{

	}
	ConfigParse::ConfigParse(string s)
	{
		parse(s);
	}
	template <typename TN>
	bool ConfigParse::insert(vector<string> path,TN a)
	{
		Node<TN>* now = getRoot<TN>();
		for (unsigned int i = 0;i<path.size()-1;i++)
		{
			bool find = false;
			for (unsigned int j = 0;j<now->sonNode.size();j++)
				if (now->sonNode[j].name==path[i])
				{
					now = &(now->sonNode[j]);
					find = true;
					break;
				}
			if (!find)
			{
				for (unsigned int j = i;j<path.size()-1;j++)
				{
					now = &(now->sonNode[now->createSon(path[j])]);
				}
				now->createLeaf(path[path.size()-1],a);
				return true;
			}
		}
		bool find = false;
		for (unsigned int i = 0;i<now->leafs.size();i++)
		if (now->leafs[i].name == path[path.size()-1])
		{
			now->leafs[i].value = a;
			return true;
		}
		now->createLeaf(path[path.size()-1],a);
		return true;
	}
	template <typename TN>
	bool ConfigParse::getValue(vector<string> path,TN& a)
	{
		Node<TN>* now = getRoot<TN>();
		for (unsigned int i = 0;i<path.size()-1;i++)
		{
			bool find = false;
			for (unsigned int j = 0;j<now->sonNode.size();j++)
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
		for (unsigned int i = 0;i<now->leafs.size();i++)
			if (now->leafs[i].name == path[path.size()-1])
			{
				a= now->leafs[i].value;
				return true;
			}
			return false;
	}
	template <typename TN>
	bool ConfigParse::getValue(string s,TN& a)
	{
		vector<string> svector;
		s = s + '.';
		string s1="";
		for (unsigned int i = 0;i<s.length();i++)
		{
			if (s[i] == '.')
			{
				svector.push_back(s1);
				s1 = "";
			}
			else
				s1 += s[i];
		}
		return getValue(svector,a);
	}
	template <typename TN>
	Node<TN>* ConfigParse::getRoot()
	{
		if (typeid(TN)==typeid(int))
			return (Node<TN>*)&root_int;
		if (typeid(TN)==typeid(float))
			return (Node<TN>*)&root_float;
		if (typeid(TN)==typeid(double))
			return (Node<TN>*)&root_double;
		if (typeid(TN)==typeid(bool))
			return (Node<TN>*)&root_bool;
		if (typeid(TN)==typeid(string))
			return (Node<TN>*)&root_string;
		if (typeid(TN)==typeid(vector<int>))
			return (Node<TN>*)&root_vector_int;
		if (typeid(TN)==typeid(vector<double>))
			return (Node<TN>*)&root_vector_double;
		if (typeid(TN)==typeid(vector<float>))
			return (Node<TN>*)&root_vector_float;
		if (typeid(TN)==typeid(vector<bool>))
			return (Node<TN>*)&root_vector_bool;
		if (typeid(TN)==typeid(vector<string>))
			return (Node<TN>*)&root_vector_string;
		return NULL;
	}
};