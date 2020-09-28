#include "UpdateMasterInterface.h"
#include "UpdateSlaveInterface.h"

UpdateMasterInterface::UpdateMasterInterface(void)
{
}


UpdateMasterInterface::~UpdateMasterInterface(void)
{
}

void UpdateMasterInterface::AddSlave( const UpdateSlaveInterface* pUSI )
{
	m_UpdateSlaveInterfaceVector.push_back(pUSI);
}

void UpdateMasterInterface::UpdateSlave()
{
	for(int i = 0; i < m_UpdateSlaveInterfaceVector.size(); i++)
	{
		UpdateSlaveInterface* pUSI = (UpdateSlaveInterface*)(m_UpdateSlaveInterfaceVector.at(i));
		pUSI->UpdateMe();
	}
}
