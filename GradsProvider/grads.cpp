#include "grads.h"
#include "galloc.h"

struct gacmn gcmn;
struct gawgds wgds;
struct gamfcmn mfcmn;

/* query the global cache scale factor */
gadouble qcachesf (void) {
	return(1.0);
}

/* Initialize most gacmn values.  Values involving real page size,
and values involving open files, are not modified   */

void gainit (void) {
	gaint i;

	gcmn.wgds = &wgds;
	gcmn.wgds->fname = NULL;
	gcmn.wgds->opts = NULL;
	gcmn.hbufsz = 1000000;
	gcmn.g2bufsz = 10000000;
	gcmn.loopdim = 3;
	gcmn.csmth = 0;
	gcmn.cterp = 1;
	gcmn.cint = 0;
	gcmn.cflag = 0;
	gcmn.ccflg = 0;
	gcmn.cmin = -9.99e33;
	gcmn.cmax = 9.99e33;
	gcmn.arrflg = 0;
	gcmn.arlflg = 1;
	gcmn.ahdsiz = 0.05;
	gcmn.hemflg = -1;
	gcmn.aflag = 0;
	gcmn.axflg = 0;
	gcmn.ayflg = 0;
	gcmn.rotate = 0;
	gcmn.xflip = 0;
	gcmn.yflip = 0;
	gcmn.gridln = -9;
	gcmn.zlog = 0;
	gcmn.coslat = 0;
	gcmn.numgrd = 0;
	gcmn.gout0 = 0;
	gcmn.gout1 = 1;
	gcmn.gout1a = 0;
	gcmn.gout2a = 1;
	gcmn.gout2b = 4;
	gcmn.goutstn = 1;
	gcmn.cmark = -9;
	gcmn.grflag = 1;
	gcmn.grstyl = 5;
	gcmn.grcolr = 15;
	gcmn.blkflg = 0;
	gcmn.dignum = 0;
	gcmn.digsiz = 0.07;
	gcmn.reccol = 1;
	gcmn.recthk = 3;
	gcmn.lincol = 1;
	gcmn.linstl = 1;
	gcmn.linthk = 3;

	gcmn.mproj = 2;
	gcmn.mpdraw = 1;
	gcmn.mpflg = 0;
	gcmn.mapcol = -9; gcmn.mapstl = 1; gcmn.mapthk = 1;
	for (i=0; i<256; i++) {
		gcmn.mpcols[i] = -9;
		gcmn.mpstls[i] = 1;
		gcmn.mpthks[i] = 3;
	}
	gcmn.mpcols[0] = -1;  gcmn.mpcols[1] = -1; gcmn.mpcols[2] = -1;

	for (int i=0;i<8;i++)
	{
		if (gcmn.mpdset[i])
		{
			delete gcmn.mpdset[i];
			gcmn.mpdset[i] = NULL;
		}
	}

	gcmn.mpdset[0] = (char *)galloc(7,"mpdset0");
	*(gcmn.mpdset[0]+0) = 'l'; 
	*(gcmn.mpdset[0]+1) = 'o'; 
	*(gcmn.mpdset[0]+2) = 'w'; 
	*(gcmn.mpdset[0]+3) = 'r'; 
	*(gcmn.mpdset[0]+4) = 'e'; 
	*(gcmn.mpdset[0]+5) = 's'; 
	*(gcmn.mpdset[0]+6) = '\0';
	for (i=1;i<8;i++) gcmn.mpdset[i]=NULL;

	gcmn.strcol = 1;
	gcmn.strthk = 3;
	gcmn.strjst = 0;
	gcmn.strrot = 0.0;
	gcmn.strhsz = 0.1;
	gcmn.strvsz = 0.12;
	gcmn.anncol = 1;
	gcmn.annthk = 6;
	gcmn.tlsupp = 0;
	gcmn.xlcol = 1;
	gcmn.ylcol = 1;
	gcmn.xlthck = 4;
	gcmn.ylthck = 4;
	gcmn.xlsiz = 0.11;
	gcmn.ylsiz = 0.11;
	gcmn.xlflg = 0;
	gcmn.ylflg = 0;
	gcmn.xtick = 1;
	gcmn.ytick = 1;
	gcmn.xlint = 0.0;
	gcmn.ylint = 0.0;
	gcmn.xlpos = 0.0;
	gcmn.ylpos = 0.0;
	gcmn.ylpflg = 0;
	gcmn.yllow = 0.0;
	gcmn.xlside = 0;
	gcmn.ylside = 0;
	gcmn.clsiz = 0.09;
	gcmn.clcol = -1;
	gcmn.clthck = -1;
	gcmn.stidflg = 0;
	gcmn.grdsflg = 1;
	gcmn.timelabflg = 1;
	gcmn.stnprintflg = 0;
	gcmn.fgcnt = 0;
	gcmn.barflg = 0;
	gcmn.bargap = 0;
	gcmn.barolin = 0;
	gcmn.clab = 1;
	gcmn.clskip = 1;
	gcmn.xlab = 1;
	gcmn.ylab = 1;
	gcmn.clstr = NULL;
	gcmn.xlstr = NULL;
	gcmn.ylstr = NULL;
	gcmn.xlabs = NULL;
	gcmn.ylabs = NULL;
	gcmn.dbflg = 0;
	gcmn.rainmn = 0.0;
	gcmn.rainmx = 0.0;
	gcmn.rbflg = 0;
	gcmn.miconn = 0;
	gcmn.impflg = 0;
	gcmn.impcmd = 1;
	gcmn.strmden = 5;
	gcmn.frame = 1;
	gcmn.pxsize = gcmn.xsiz;
	gcmn.pysize = gcmn.ysiz;
	gcmn.vpflag = 0;
	gcmn.xsiz1 = 0.0;
	gcmn.xsiz2 = gcmn.xsiz;
	gcmn.ysiz1 = 0.0;
	gcmn.ysiz2 = gcmn.ysiz;
	gcmn.paflg = 0;
	for (i=0; i<10; i++) gcmn.gpass[i] = 0;
	gcmn.btnfc = 1;
	gcmn.btnbc = 0;
	gcmn.btnoc = 1;
	gcmn.btnoc2 = 1;
	gcmn.btnftc = 1;
	gcmn.btnbtc = 0;
	gcmn.btnotc = 1;
	gcmn.btnotc2 = 1;
	gcmn.btnthk = 3;
	gcmn.dlgpc = -1;
	gcmn.dlgfc = -1;
	gcmn.dlgbc = -1;
	gcmn.dlgoc = -1;
	gcmn.dlgth = 3;
	gcmn.dlgnu = 0;
	for (i=0; i<15; i++) gcmn.drvals[i] = 1;
	gcmn.drvals[1] = 0; gcmn.drvals[5] = 0;
	gcmn.drvals[9] = 0;
	gcmn.drvals[14] = 1;
	gcmn.sig = 0;
	gcmn.lfc1 = 2;
	gcmn.lfc2 = 3;
	gcmn.wxcols[0] = 2; gcmn.wxcols[1] = 10; gcmn.wxcols[2] = 11;
	gcmn.wxcols[3] = 7; gcmn.wxcols[4] = 15;
	gcmn.wxopt = 1;
	for (i=0; i<32; i++) gcmn.clct[i] = NULL;
	gcmn.ptflg = 0;
	gcmn.ptopt = 1;
	gcmn.ptden = 5;
	gcmn.ptang = 0;
	gcmn.statflg = 0;
	gcmn.prstr = NULL;  gcmn.prlnum = 8; 
	gcmn.prbnum = 1; gcmn.prudef = 0;
	gcmn.dwrnflg = 1;
	gcmn.xexflg = 0; gcmn.yexflg = 0;
	gcmn.cachesf = 1.0; 
	gcmn.dblen = 12;
	gcmn.dbprec = 6;
	gcmn.bSpecialZ = false; //wangjl 2012.1.12

	mfcmn.cal365=-999;
	mfcmn.warnflg=2;
	mfcmn.winx=-999;      /* Window x  */         
	mfcmn.winy=-999;      /* Window y */     
	mfcmn.winw=0;         /* Window width */ 
	mfcmn.winh=0;         /* Window height */ 
	mfcmn.winb=0;         /* Window border width */

}

void gasig(gaint i)
{
	if (gcmn.sig) exit(0);
	gcmn.sig = 1;
}

gaint gaqsig (void) 
{
	return(gcmn.sig);
}