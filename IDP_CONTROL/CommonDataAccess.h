#ifndef COMMONDATAACCESS_H
#define COMMONDATAACCESS_H

#include "idp_control_global.h"

#include <QObject>

class IDP_CONTROL_EXPORT CommonDataAccess : public QObject
{
public:
	CommonDataAccess(QObject *parent = 0);
	~CommonDataAccess();

	static CommonDataAccess *Instance();//得到操作类实例
private:
	
};

#endif // COMMONDATAACCESS_H
