#ifndef CPROFILER_H
#define CPROFILER_H

#include <windows.h>
#include <vector>
#include <string>
using namespace std;
struct ID
{
	string FunctionName;
	int IDnum;
};
struct Profile
{
	int  numcycles;
	LARGE_INTEGER start;	
	double Total;
	double Longest;
	double Shortest;
	ID id;
};
class CProfiler
{
	LARGE_INTEGER freq;
	vector<Profile*> profiles;
	static CProfiler* profiler;
	CProfiler(CProfiler& copy){}
	CProfiler();
	~CProfiler();
public:

	static CProfiler* GetInstance()
	{
		if(profiler == NULL)
		{
			profiler = new CProfiler;
		}
		return profiler;
	}
	void DeleteInstance()
	{
		if(profiler)
		{
			delete profiler;
			profiler = NULL;
		}

	}
	
	void Start(string FuncName);
	void End(string FuncName);
	void Save(string FileName);
};

#endif