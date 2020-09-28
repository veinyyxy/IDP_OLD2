#pragma once
#include "Controller.h"
#include "ParamSwapModel.h"
#include "TreeViewEventInterface.h"

class MapLayerTreeViewController : public Controller
{
public:
	MapLayerTreeViewController(void);
	~MapLayerTreeViewController(void);

	virtual int PreExcute();

	virtual int Excute();

	virtual int PostExcute();

	virtual void GetName( std::string& strID );

	virtual void Input( const FrameModel* pFM );

	virtual FrameModel* Output();
private:
	ParamSwapModel m_SwapModel;
	TreeViewEventInterface* m_pTreeViewEvent;
};

