#pragma once
#include "idp_control_global.h"

class QStringList;
class AnimaticControlerInterface;
class AnimaticDataLoader;
class ViewManager;
class QWidget;

class IDP_CONTROL_EXPORT AnimaticControler
{
public:
	AnimaticControler(osgViewer::View* pVM);
	
	virtual ~AnimaticControler(void);
	bool IsInitComplete();
	void InitData(QString path);
	void Play();
	void Stop();
	void Loop();
	void Pause();
	void SpeedUp();
	void SpeedDown();
	void Next();
	void Previous();
	void LoadDataFromNameList(const QStringList& strList);
	inline QWidget* GetAnimaticWindow(){return m_pWidget;}
	inline AnimaticControlerInterface* GetAnimaticControlerInterface(){return m_pRadarAnimatic;}
	inline void SetLoadFileYes(bool bLFY){m_LoadFileYes = bLFY;}

private:
	AnimaticDataLoader* m_pAnimaticDataLoader;
	AnimaticControlerInterface* m_pRadarAnimatic;
	bool m_LoadFileYes;
	QWidget* m_pWidget;
};

