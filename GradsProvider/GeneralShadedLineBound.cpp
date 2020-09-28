#include "GeneralShadedLineBound.h"
#include "gxX.h"
#include "gauser.h"

GeneralShadedLineBound::GeneralShadedLineBound(void)
{
}


GeneralShadedLineBound::~GeneralShadedLineBound(void)
{
}

int GeneralShadedLineBound::Dump( goto_gis::Layer* pLayer )
{
	return 0;
}

int GeneralShadedLineBound::Draw()
{
	return 0;
}

void GeneralShadedLineBound::InitCommand()
{
	gcmn.sig = 0;
	gacmd("set csmooth on",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set cterp off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set grid off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set mpdraw off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set frame off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set clab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set grads off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set xlab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set ylab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set arrlab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set lon 0 360",&gcmn,0);   // added by Liu Xinsheng 2011-05-02
	gcmn.sig = 0;
	gacmd("set lat -89 89",&gcmn,0);     // added by WangJL 2011-05-02
	gcmn.sig = 0;
	gacmd("set cstyle 1",&gcmn,0);

	for (int i = 16;i < 64;i++)
	{
		sprintf(m_Command,"set rgb");

		sprintf(m_Temp," %d",i);
		strcat(m_Command,m_Temp);

		sprintf(m_Temp," %d",i*4);
		strcat(m_Command,m_Temp);

		sprintf(m_Temp," %d",i*4);
		strcat(m_Command,m_Temp);

		sprintf(m_Temp," %d",i*4);
		strcat(m_Command,m_Temp);

		gcmn.sig = 0;
		gacmd(m_Command,&gcmn,0);
	}

}