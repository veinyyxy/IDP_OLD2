#include "fmdata.h"

using namespace std;

namespace FMDATA
{

void Grid3dgHead::read(std::ifstream& fin)
{
	char line[1024]={0}; 
	int nRow = 1;
	while(nRow<=28 && fin.getline(line, sizeof(line)))
	{
		if (nRow==4)
		{
			std::stringstream word(line); 
			word>>m_layersize;
		}
		m_strHead.push_back(string(line));
		nRow++;
	}
}

void Grid3dgHead::write(FILE* fp)
{
	for (int i=0;i<m_strHead.size();i++)
	{
		fprintf(fp,"%s\n",m_strHead.at(i).c_str());
	}
}

//////////////////////////////////////////////////////////////////////////

void VelocityGrid::read(std::ifstream& fin)
{
	char line[1024]={0}; 
	int nRow = 1;
	int nRowMax = 31;
	while(nRow<=nRowMax && fin.getline(line, sizeof(line)))
	{
		m_strContend.push_back(string(line));
		if (nRow==8)
		{
			std::stringstream word(line); 
			word>>m_strVelocityfile;
		}
		if (nRow==31)
		{
			std::stringstream word(line); 
			word>>m_Nspikes;
			nRowMax +=m_Nspikes*2;
		}
		nRow++;
	}
}

void VelocityGrid::write(FILE* fp)
{
	for (int i=0;i<m_strContend.size();i++)
	{
		fprintf(fp,"%s\n",m_strContend.at(i).c_str());
	}
}

//////////////////////////////////////////////////////////////////////////
void interfaceProPerty::read(std::ifstream& fin)
{
	char line[1024]={0}; 
	int nRow = 1;
	int nRowMax = 27;
	while(nRow<=nRowMax && fin.getline(line, sizeof(line)))
	{
		m_strContend.push_back(string(line));
		if (nRow==6)
		{
			std::stringstream word(line); 
			word>>m_depthNW;
		}
		if (nRow==7)
		{
			std::stringstream word(line); 
			word>>m_depthNE;
		}
		if (nRow==8)
		{
			std::stringstream word(line); 
			word>>m_depthSW;
		}
		if (nRow==19)
		{
			std::stringstream word(line); 
			word>>m_checkBord;
		}
		if (nRow==27)
		{
			std::stringstream word(line); 
			word>>m_Nspikes;
			nRowMax +=m_Nspikes*2;
		}
		nRow++;
	}
}

void interfaceProPerty::write(FILE* fp)
{
	for (int i=0;i<m_strContend.size();i++)
	{
		fprintf(fp,"%s\n",m_strContend.at(i).c_str());
	}
}


//////////////////////////////////////////////////////////////////////////

Grid3dg::Grid3dg(const char* filename)
{

}

bool Grid3dg::read(const char* filename)
{
	std::ifstream fin(filename,std::ios::in); 
	char line[1024]={0}; 

	m_grid3dghead.read(fin);
	//line 29-32
	fin.getline(line, sizeof(line));
	m_strInterfaceHead.push_back(string(line));
	fin.getline(line, sizeof(line));
	m_strInterfaceHead.push_back(string(line));
	fin.getline(line, sizeof(line));
	m_strInterfaceHead.push_back(string(line));
    
	int Layersize = m_grid3dghead.LayerSize();
	
	for(int i=0;i<Layersize;i++)
	{
		VelocityGrid grid;
		grid.read(fin);
		m_vGrid.push_back(grid);
	}
	//line 100 end of vgrid
	fin.getline(line, sizeof(line));
	m_vgridendstr = string(line);

	///line101-106
	for (int i=0;i<6;i++)
	{
		fin.getline(line, sizeof(line));
		m_strInterfaceHead2.push_back(string(line));
		if(i==4)
		{
			std::stringstream word(line);
			word>>m_nNS;
		}
		if(i==5)
		{
			std::stringstream word(line);
			word>>m_nEW;
		}
	}

	//107-199,(107-137,138-168,169-199)
	for (int i=0;i<3;i++)
	{
	   interfaceProPerty iproperty;
	   iproperty.read(fin);
	   m_iGrid.push_back(iproperty); 
	}
	return true;
}

bool Grid3dg::write(const char* filename)
{
	FILE* fp;
	if ((fp=fopen(filename,"w"))==NULL)
	{
		return false;
	}

	m_grid3dghead.write(fp);

	for (int i=0;i<m_strInterfaceHead.size();i++)
	{
		fprintf(fp,"%s\n",m_strInterfaceHead.at(i).c_str());
	}

	for(int i=0;i<m_vGrid.size();i++)
	{
		m_vGrid.at(i).write(fp);
	}

	fprintf(fp,"cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n");


	for (int i=0;i<m_strInterfaceHead2.size();i++)
	{
		fprintf(fp,"%s\n",m_strInterfaceHead2.at(i).c_str());
	}

	for (int i=0;i<m_iGrid.size();i++)
	{
		m_iGrid.at(i).write(fp); 
	}

	fclose(fp);

	return true;

}
}