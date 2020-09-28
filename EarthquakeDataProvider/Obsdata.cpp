#include "fmdata.h"

namespace FMDATA
{
void PathSegments::Read(std::ifstream& fin)
{
	char line[1024]={0};
	int nRow = 1;
	int nRowMax = 4;
	while(nRow<=nRowMax && fin.getline(line, sizeof(line)))
	{
		m_StrdataAll.push_back(string(line));
		nRow++;
	}
}

void PathSegments::Write(FILE* fp)
{
	for (int i=0;i<m_StrdataAll.size();i++)
	{
		fprintf(fp,"%s\n",m_StrdataAll.at(i).c_str());
	}
}

void PathSegments::Write(std::ifstream& fin)
{
	char line[1024]={0};
	//int nRow = 1;
	//int nRowMax = 4;
	/*while(nRow<=nRowMax && fin.getline(line, sizeof(line)))*/
// 	for (int i=0;i<m_StrdataAll.size();i++)
// 	{
// 		fin<<m_StrdataAll.at(i).c_str();
// 	}
	 
}

//////////////////////////////////////////////////////////////////////////
void SourceFileInfo::Read(std::ifstream& fin)
{
	char line[1024]={0};
	int nRow = 1;
	int nRowMax = 7;
	while(nRow<=nRowMax && fin.getline(line, sizeof(line)))
	{
		m_StrdataAll.push_back(string(line));
		if (nRow==7)
		{
			std::stringstream word(line); 
			word>>m_pathsegNum;
		}
		nRow++;
	}

	for (int i=0;i<m_pathsegNum;i++)
	{
		PathSegments segs;
		segs.Read(fin);
		m_PathSegs.push_back(segs);
	}
}

void SourceFileInfo::Write(FILE* fp)
{
	for (int i=0;i<m_StrdataAll.size();i++)
	{
		fprintf(fp,"%s\n",m_StrdataAll.at(i).c_str());
	}
	for (int i=0;i<m_PathSegs.size();i++)
	{
		m_PathSegs.at(i).Write(fp);
	}
}

//////////////////////////////////////////////////////////////////////////

bool Obsdata::read(const char* filename)
{
	clear();

	std::ifstream fin(filename,std::ios::in); 
	char line[1024]={0}; 

	int headrow = 1;
	int headrowmax = 12;

	while(headrow<=headrowmax && fin.getline(line, sizeof(line)))
	{
		m_flehead.push_back(string(line));
		if (headrow==10)
		{
			std::stringstream word(line); 
			word>>m_sourcefileNum;   
		}
		headrow++;
	}

	for (int i=0;i<m_sourcefileNum;i++)
	{
		SourceFileInfo srcinfo;
		srcinfo.Read(fin);
		m_srcfleInfo.push_back(srcinfo);
	}

	return true;
}

void Obsdata::Write(const char* filename)
{
	FILE* fp;
	if ((fp=fopen(filename,"w"))==NULL)
	{
		return;
	}

	for (int i=0;i<m_flehead.size();i++)
	{
		fprintf(fp,"%s\n",m_flehead.at(i).c_str());
	}
	for (int i=0;i<m_srcfleInfo.size();i++)
	{
		m_srcfleInfo.at(i).Write(fp);
	}
	fclose(fp);
}

void Obsdata::clear()
{
	m_flehead.clear();
	m_srcfleInfo.clear();
 }
}