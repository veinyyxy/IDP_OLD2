
#include "CommonDataAccess.h"

CommonDataAccess::CommonDataAccess(QObject *parent)
	: QObject(parent)
{

}

CommonDataAccess::~CommonDataAccess()
{

}

Q_GLOBAL_STATIC(CommonDataAccess, commonDataAccess)
	CommonDataAccess *CommonDataAccess::Instance() 
{ 
	return commonDataAccess(); 
}
