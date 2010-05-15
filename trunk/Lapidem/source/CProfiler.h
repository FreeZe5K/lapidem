#ifndef CPROFILER_H
#define CPROFILER_H
#include "stdheaders.h"

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
		/*if(profiler == NULL)
		{
			profiler = new CProfiler;
		}*/
		static CProfiler new_CP;
		return &new_CP;//profiler;
	}
	void DeleteInstance()
	{
		
	}
	
	void Start(string FuncName);
	void End(string FuncName);
	void Save(string FileName);
};

#endif