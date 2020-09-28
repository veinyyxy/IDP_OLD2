#pragma once

class AnimaticControlerInterface
{
public:
	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;
	virtual void Previous() = 0;
	virtual void Next() = 0;
	virtual void Loop() = 0;
	virtual void SpeedDown() = 0;
	virtual void SpeedUp() = 0;
	virtual void SetStartFrame(int iSF) = 0;
	virtual void SetEndFrame(int iEF) = 0;
protected:
	int m_iStartFrame;
	int m_iEndFrame;
};