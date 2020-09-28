#pragma once
class UpdateSlaveInterface;
#include "idpui_global.h"
#include <QVector>
class IDPUI_EXPORT UpdateMasterInterface
{
public:
	UpdateMasterInterface(void);
	~UpdateMasterInterface(void);
	virtual void AddSlave( const UpdateSlaveInterface* pUSI );
	virtual void UpdateSlave();
private:
	QVector<const UpdateSlaveInterface*> m_UpdateSlaveInterfaceVector;
};

