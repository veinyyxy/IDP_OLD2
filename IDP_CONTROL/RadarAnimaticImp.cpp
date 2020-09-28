#include <QtSql/QtSql>
#include <QtCore/QtCore>
#include <QtCore/QMutex>
#include <QtGui/QTreeWidgetItem>
#include "Map.h"
#include "AnimaticFrame.h"
#include <windows.h>
#include "AnimaticControlerInterface.h"
#include "RadarAnimaticImp.h"

RadarAnimaticImp::RadarAnimaticImp(double dSppeed) 
	: m_PlaySpeed(dSppeed * 1000000)
	, m_bInitComplete(false)
{
	m_iStartFrame = 0;
	m_iEndFrame = -1;
	m_Count = 0;
	m_LayerCount = 0;
	m_AnimaticState = ANIMATIC_Stop;
}

RadarAnimaticImp::~RadarAnimaticImp()
{

}

void RadarAnimaticImp::Play()
{
	//m_Mutex.lock();
	//if(!IsInitComplete()) return;
	m_AnimaticState = ANIMATIC_Play;
	//m_Mutex.unlock();
	if((m_iStartFrame < 0) || m_iStartFrame > m_AnimaticBuff.size() - 2)
		m_iStartFrame = 0;

	if((m_iEndFrame < 0) || (m_iEndFrame > m_AnimaticBuff.size() - 1))
		m_iEndFrame = m_AnimaticBuff.size();
	start();
}

void RadarAnimaticImp::Pause()
{
	//if(!IsInitComplete()) return;
	//m_Mutex.lock();
	m_AnimaticState = ANIMATIC_Stop;
	//m_Mutex.unlock();
}

void RadarAnimaticImp::Stop()
{
	//if(!IsInitComplete()) return;
	//m_Mutex.lock();
	m_pMutex.lock();
	m_AnimaticState = ANIMATIC_Stop;
	m_Count = m_iStartFrame;
	m_pMutex.unlock();
	//m_Mutex.unlock();
}

void RadarAnimaticImp::Previous()
{
	//if(!IsInitComplete()) return;
	m_pMutex.lock();
	m_Count--;
	if(m_Count < m_iStartFrame)
		m_Count = m_iStartFrame;
	m_pMutex.unlock();
}

void RadarAnimaticImp::Next()
{
	//if(!IsInitComplete()) return;
	m_pMutex.lock();
	m_Count++;
	if(m_Count > m_iEndFrame)
		m_Count = m_iEndFrame;
	m_pMutex.unlock();
}

void RadarAnimaticImp::Loop()
{
	//if(!IsInitComplete()) return;
	//m_Mutex.lock();
	m_AnimaticState = ANIMATIC_Loop;
	//m_Mutex.unlock();
	if((m_iStartFrame < 0) || m_iStartFrame > m_AnimaticBuff.size() - 2)
		m_iStartFrame = 0;

	if((m_iEndFrame < 0) || (m_iEndFrame > m_AnimaticBuff.size() - 1))
		m_iEndFrame = m_AnimaticBuff.size();

	start();
}

void RadarAnimaticImp::SpeedDown()
{
	//if(!IsInitComplete()) return;
	m_PlaySpeed -= (0.05 * 1000000);
}

void RadarAnimaticImp::SpeedUp()
{
	//if(!IsInitComplete()) return;
	m_PlaySpeed += (0.05 * 1000000);
}

void RadarAnimaticImp::run()
{
	int m_iNextCount = 0;
	LARGE_INTEGER litmp;
	LONGLONG Qpart1,Qpart2,Useingtime;
	double dfMinus,dfFreq,dfTime;

	if(!m_AnimaticBuff.size()) return;
	
	while(1)
	{
		//m_Mutex.lock();
		switch(m_AnimaticState)
		{
		case ANIMATIC_Play:
			if(m_iEndFrame == m_Count)
				Stop();
			break;
		case ANIMATIC_Pause:
		case ANIMATIC_Stop:
			//m_Mutex.unlock();
			return;
		case ANIMATIC_Prepicture:
			break;
		case ANIMATIC_Pre:
			break;
		case ANIMATIC_Next:
			break;
		case ANIMATIC_Nextpicture:
			break;
		case ANIMATIC_Loop:
			if(m_Count == m_iEndFrame)
				m_Count = m_iStartFrame;
			break;
		case ANIMATIC_None:
			break;
		default:
			break;
		}
		//m_Mutex.unlock();
		//���CPU��ʱ����ʱ��Ƶ��
		QueryPerformanceFrequency(&litmp);//ȡ�ø߾������м�������Ƶ��f,��λ��ÿ����ٴΣ�n/s����
		dfFreq = (double)litmp.QuadPart;
		QueryPerformanceCounter(&litmp);//ȡ�ø߾������м���������ֵ
		Qpart1 = litmp.QuadPart; //��ʼ��ʱ
		do 
		{
			QueryPerformanceCounter(&litmp);//ȡ�ø߾������м���������ֵ
			Qpart2 = litmp.QuadPart; //��ֹ��ʱ

			dfMinus = (double)(Qpart2 - Qpart1);//���������ֵ
			dfTime = dfMinus / dfFreq;//��ö�Ӧʱ�䣬��λΪ��,���Գ�1000000��ȷ��΢�뼶��us��
			dfTime *= 1000000;
		} while(dfTime < m_PlaySpeed);

		m_pMutex.lock();
		//if(m_Count >= m_AnimaticBuff.size() || m_Count) Stop();
		m_AnimaticBuff.value(m_Count)->Play();
		m_Count++;
		m_pMutex.unlock();
	}
	
}

void RadarAnimaticImp::SetStartFrame( int iSF )
{
	m_pMutex.lock();
	m_iStartFrame = iSF;
	if((m_iStartFrame < 0) || m_iStartFrame > m_AnimaticBuff.size() - 2)
		m_iStartFrame = 0;
	m_pMutex.unlock();
}

void RadarAnimaticImp::SetEndFrame( int iEF )
{
	m_pMutex.lock();
	m_iEndFrame = iEF;
	if((m_iEndFrame < 0) || (m_iEndFrame > m_AnimaticBuff.size() - 1))
		m_iEndFrame = m_AnimaticBuff.size();
	m_pMutex.unlock();
}


