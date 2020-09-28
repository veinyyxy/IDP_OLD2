
#include "DataProvider.h"
#include "Layer.h"

GOTO_NAMESPACE_START

DataProvider::DataProvider():m_ColorTable(0),m_productcode(0)
{
	m_pParentLayer = 0;
	m_edataType = NO_TYPE;
	m_symbolLegend = 0;
	m_symbolLegend = new SymbolLegend_Vector;
	/*SymbolLegend* ss1 = new SymbolLegend;
	ss1->labelText="0-111";
	ss1->pixmap = QPixmap("D:/workspace/IDP/IDPUI/images/logo.png");
	m_symbolLegend->append(ss1);

	SymbolLegend* ss2 = new SymbolLegend;
	ss2->labelText="0-222";
	ss2->pixmap = QPixmap("D:/workspace/IDP/IDPUI/images/logo.png");
	m_symbolLegend->append(ss2);

	SymbolLegend* ss3 = new SymbolLegend;
	ss3->labelText="0-333";
	ss3->pixmap = QPixmap("D:/workspace/IDP/IDPUI/images/logo.png");
	m_symbolLegend->append(ss3);*/
}


DataProvider::~DataProvider()
{

}

void DataProvider::InsertExtendInterface( void* pIF )
{
	size_t iI = m_ExtendInterface.size();
	m_ExtendInterface.insert(INDEX_INTERFACE_PAIR(iI, pIF));
}

void* DataProvider::GetExtendInterface( int iI )
{
	INDEX_INTERFACE_MAP::iterator fr = m_ExtendInterface.find(iI);
	if(fr != m_ExtendInterface.end())
	{
		return fr->second;
	}
	else
		return 0;
}

bool DataProvider::MineData(const std::string&)
{
	return false;
}

GOTO_NAMESPACE_END