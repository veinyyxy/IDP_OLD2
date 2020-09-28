#pragma once
#include <QThread>

class AnimaticFrame;
class QMutex;
class AnimaticControlerInterface;

#define FRAME_COUNT1 0.037037
#define FRAME_COUNT2 0.2

class RadarAnimaticImp : public AnimaticControlerInterface, public QThread
{
public:
	typedef enum __state_animatic
	{
		ANIMATIC_None = 0,
		ANIMATIC_Play, 
		ANIMATIC_Pause,
		ANIMATIC_Stop,
		ANIMATIC_Prepicture,
		ANIMATIC_Pre,
		ANIMATIC_Next,
		ANIMATIC_Nextpicture,
		ANIMATIC_Loop,
	} ANIMATIC_STATE;
	typedef QMap<int, AnimaticFrame*> FRAME_BUFFER, *P_FRAME_BUFFER;
	RadarAnimaticImp(double dSppeed = FRAME_COUNT2);
	~RadarAnimaticImp();
	int InitData();
	void Play();
	void Pause();
	void Stop();
	void Previous();
	void Next();
	void Loop();
	void SpeedDown();
	void SpeedUp();
	void run();
	inline bool IsInitComplete(){return m_bInitComplete;}
	//inline void setMutex(const QMutex* pM){m_pMutex = (QMutex*)pM;}
	//inline QMutex* getMutex(){return m_pMutex;}
	inline void SetFrameBuffer(const P_FRAME_BUFFER pfb){m_AnimaticBuff = *pfb;}
	void SetStartFrame(int iSF);
	void SetEndFrame(int iEF);
private:
	//QMutex m_Mutex;
	bool m_bInitComplete;
	double m_PlaySpeed;
	int m_Count;
	int m_LayerCount;
	ANIMATIC_STATE m_AnimaticState;
	QMutex m_pMutex;

	FRAME_BUFFER m_AnimaticBuff;
};

