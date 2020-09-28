#include "sqlitemanager.h"
#include "MySQLManager.h"
#include "ODBCManager.h"
#include "OracleManager.h"
#include "dbmanager.h"

//下面是ODBC和OLEDB的连接字符串写法：
//ODBC连接
//适合数据库类型 连接方式
//access "Driver={microsoft access driver(*.mdb)};dbq=*.mdb;uid=admin;pwd=pass;"
//dBase "Driver={microsoft dbase driver(*.dbf)};driverid=277;dbq=------------;"
//oracle "Driver={microsoft odbc for oracle};server=oraclesever.world;uid=admin;pwd=pass;"
//MSSQL server "Driver={sql server};server=servername;database=dbname;uid=sa;pwd=pass;"
//MS text "Driver={microsoft text driver(*.txt; *.csv)};dbq=-----;extensions=asc,csv,tab,txt;Persist SecurityInfo=false;"
//Visual Foxpro "Driver={microsoft Visual Foxpro driver};sourcetype=DBC;sourceDB=*.dbc;Exclusive=No;"
//MySQL "Driver={mysql};database=yourdatabase;uid=username;pwd=yourpassword;option=16386;"
//SQLite "Driver={SQLite3 ODBC Driver};Database=D:\SQLite\*.db"
//PostgreSQL "Driver={PostgreSQL ANSI};server=127.0.0.1;uid=admin;pwd=pass;database=databaseName"
//OLEDB连接
//适合的数据库类型 连接方式
//access "Provider=microsoft.jet.oledb.4.0;data source=your_database_path;user id=admin;password=pass;"
//oracle "Provider=OraOLEDB.Oracle;data source=dbname;user id=admin;password=pass;"
//MS SQL Server "Provider=SQLOLEDB;data source=machinename;initial catalog=dbname;userid=sa;password=pass;"
//MS text "Provider=microsof.jet.oledb.4.0;data source=your_path;Extended Properties'text;FMT=Delimited'"

DBManager* DBManager::GetDBManager(EnumDBType dbType)
{
	if (dbType == Sqlite)
	{
		return SqliteManager::Instance();
	}
	else if (dbType == MySQL)
	{
		return MySQLManager::Instance();
	}
	else if (dbType == ODBC)
	{
		return ODBCManager::Instance();
	}
	else if (dbType == Oracle)
	{
		return OracleManager::Instance();
	}
}

Q_GLOBAL_STATIC(DBManager, dbManager)
	DBManager *DBManager::Instance() 
{ 
	return dbManager(); 
}

DBManager::DBManager()
{

}

DBManager::~DBManager()
{

}



//Q_GLOBAL_STATIC_WITH_ARGS(DBManager, dbManager,(QString, QLatin1String("/imageformats")))
//	DBManager *DBManager::Instance(const QString& dbType) 
//{ 
//	return dbManager(dbType); 
//}
