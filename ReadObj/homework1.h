#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <gl/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

typedef struct
{
	float x;
	float y;
	float z;
} VERTEX;

class Object {

public:
	Object();
	void calulate_helper();
	std::vector<VERTEX> vertexs;
	std::vector< std::pair<float,float> > texcoords;
	std::vector<int> faces;
	std::vector<VERTEX> normals;
	std::vector<VERTEX> helper;
	std::vector<VERTEX> little;
	string file;
};

void Object::calulate_helper()
{
	int length = vertexs.size();
	for (int i = 0; i < length; i++)
	{
		VERTEX temp;
		temp.x = normals[i].x;
		temp.y = normals[i].y;
		temp.z = normals[i].z;
		/*float r = sqrt(vertexs[i].x*vertexs[i].x + vertexs[i].y*vertexs[i].y + vertexs[i].z*vertexs[i].z);
		temp.x = vertexs[i].x / r;
		temp.y = vertexs[i].y / r;
		temp.z = vertexs[i].z / r;*/
		helper.push_back(temp);
		VERTEX temp2;
		temp2.x = (vertexs[i].x - temp.x) / 200;
		temp2.y = (vertexs[i].y - temp.y) / 200;
		temp2.z = (vertexs[i].z - temp.z) / 200;
		little.push_back(temp2);
	}
}

Object::Object()
{
	vertexs = vector<VERTEX>();
	texcoords = vector< std::pair<float, float> >();
	normals = vector<VERTEX>();
	helper = vector<VERTEX>();
	helper = vector<VERTEX>();
	file = ".\\tank\\tank.obj";
}

inline int power_of_two(int n)
{
	if(n<=0) return 0;
	return (n&(n-1))==0;
}

inline bool startswith(string &a,string &b)
{
	if(a.size()<b.size())
	{
		return false;
	}
	return a.substr(0,b.size())==b;
}



void ReadObj(string &cd,Object &m)
{
	ifstream in;
	string line,word;
	string file = m.file;
	char Buffer[MAX_PATH];

	if(file.find(":")!=string::npos)
	{
		cd = string(file.begin(),file.begin()+file.rfind("\\"));
	}
	else if(startswith(file,string(".\\")))
	{
		GetCurrentDirectoryA(MAX_PATH,Buffer);
		cd = Buffer+string(file.begin()+1,file.begin()+file.rfind("\\"));
	}
	else if(startswith(file,string("..\\")))
	{
		GetCurrentDirectoryA(MAX_PATH,Buffer);
		cd = Buffer;
		cd = string(cd.begin(),cd.begin()+cd.rfind("\\"));
		cd = cd + string(file.begin()+2,file.begin()+file.rfind("\\"));
		cout<<cd<<endl;
	}
	else
	{
		GetCurrentDirectoryA(MAX_PATH,Buffer);
		if(file.rfind("\\")!=string::npos)
		{
			cd = Buffer+string("\\")+string(file.begin(),file.begin()+file.rfind("\\"));
		}
		else
		{
			cd = Buffer;
		}
	}

	in.open(file.c_str());
	if(!in)
	{
		cout<<"Read obj error !"<<endl;
		exit(0);
	}
	while(getline(in,line))
	{
		if(line.size()==0||line[0]=='#') continue;
		istringstream is(line);
		is>>word;
		if(word=="v")
		{
			VERTEX p;
			is>>p.x>>p.y>>p.z;
			m.vertexs.push_back(p);
		}
		else if(word=="vt")
		{
			pair<float,float> p;
			is>>p.first>>p.second;
			m.texcoords.push_back(p);
		}
		else if(word=="vn")
		{
			VERTEX p;
			is>>p.x>>p.y>>p.z;
			m.normals.push_back(p);
		}
		else if (word == "f")
		{
			int r = 0, c = 0;
			while (is >> word)
			{
				c = count(word.begin(), word.end(), '/');
				if (c == 0)
				{
					m.faces.push_back(atoi(word.c_str()));
				}
				else if (c == 1)
				{
					m.faces.push_back(atoi(string(word.begin(), word.begin() + word.find("/")).c_str()));
					m.faces.push_back(atoi(string(word.begin() + word.find("/") + 1, word.end()).c_str()));
				}
				else if (c == 2)
				{
					int a = word.find("/");
					int b = word.find("/", a + 1);
					m.faces.push_back(atoi(string(word.begin(), word.begin() + a).c_str()));
					m.faces.push_back(atoi(string(word.begin() + a + 1, word.begin() + b).c_str()));
					m.faces.push_back(atoi(string(word.begin() + b + 1, word.end()).c_str()));
				}
				++r;
			}
		}
	}
	in.close();
	m.calulate_helper();
}

#endif