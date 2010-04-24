#include "CProfiler.h"
#include <fstream>
using namespace std;

CProfiler* CProfiler::profiler = NULL;

CProfiler::CProfiler()
{
	QueryPerformanceFrequency(&freq);
}
CProfiler::~CProfiler()
{
	for(unsigned int i =0; i <profiles.size(); i++)
	{
		delete profiles[i];
	}
}

void CProfiler::Start(std::string FuncName)
{
	for(unsigned int i =0; i < profiles.size(); i++)
	{
		if(strcmp(FuncName.c_str(), profiles[i]->id.FunctionName.c_str()) ==0)
		{
			QueryPerformanceCounter(&profiles[i]->start);
			return;
		}
	}

	Profile* newprof = new Profile;
	newprof->id.FunctionName = FuncName;
	newprof->Shortest = INT_MAX;
	newprof->Longest = 0;
	newprof->Total = 0;
	newprof->numcycles = 0;
	QueryPerformanceCounter(&newprof->start);
	profiles.push_back(newprof);
}

void CProfiler::End(string FuncName)
{
	for(unsigned int i =0; i < profiles.size(); i++)
	{
		if(strcmp(FuncName.c_str(), profiles[i]->id.FunctionName.c_str()) ==0)
		{
			LARGE_INTEGER end;
			QueryPerformanceCounter(&end);
			double timebetween =  (double)end.QuadPart- (double)profiles[i]->start.QuadPart;
			profiles[i]->Total += timebetween;
			profiles[i]->numcycles++;
			if(double(profiles[i]->Longest) < double(timebetween))
			{
				profiles[i]->Longest = timebetween;
			}
			if(double(profiles[i]->Shortest) > timebetween)
			{
				profiles[i]->Shortest = timebetween;
			}
			break;
		}
	}
}

void CProfiler::Save(std::string FileName)
{
	ofstream out( FileName.c_str( ) );

	if( !out.is_open( ) )
		out.open( FileName.c_str( ), ios_base::out | ios_base::trunc );
	for(unsigned int i =0; i < profiles.size(); i++)
	{
		out << "Function Name: "<< profiles[i]->id.FunctionName.c_str() <<'\n';
		double _avg = profiles[i]->Total/ double(profiles[i]->numcycles);		
		out << "Avg execution: " << _avg << "\tNumber of Cycles: " << profiles[i]->numcycles << '\n';
		out << "Total Ticks: " << (int)profiles[i]->Total << '\n';
		out << "Shortest: " << profiles[i]->Shortest << "\tLongest: " << profiles[i]->Longest <<'\n'<< '\n';		
	}
	out.close( );
}

