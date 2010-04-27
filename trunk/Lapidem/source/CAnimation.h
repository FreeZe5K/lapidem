#ifndef CANIMATION_H
#define CANIMATION_H

#include <windows.h>
#include <vector>
using std::vector;

struct Frame
{
	RECT CollisionRect;
	RECT DrawRect;
	POINT AnchorPoint;
	char* Trigger;
};

class CAnimation
{
	int m_nImageID;
	bool m_bIsLooping;
	bool m_bIsPlaying;
	double m_dTimeToWait;
	float m_fTimeWaited; 
	int m_nFrame;
	vector <Frame*> m_vFrames;

public:
	void SetImageID(int nID) { m_nImageID = nID;}
	void SetLooping(bool bLoop) { m_bIsLooping = bLoop;}
	void SetPlaying(bool bPlaying) { m_bIsPlaying = bPlaying;}
	void SetTimeToWait(double dTTW) { m_dTimeToWait = dTTW;}
	void SetFrames(vector<Frame*> frames) {m_vFrames = frames;}

	CAnimation();
	~CAnimation();
	void Update(float fElapsedTime);
	void Play()
	{
		m_bIsPlaying = !m_bIsPlaying;
	}
	void Stop()
	{
		m_bIsPlaying = false;
		m_nFrame = 0;

	}
	void Resume()
	{
		m_bIsPlaying = true;
	}

	Frame* GetFrames(void) {return m_vFrames[m_nFrame];}
	int GetFrame(void) {return m_nFrame;}
	int GetImageID(void) {return m_nImageID;}

};

#endif