#pragma  once

#include "fmdata.h"
#include <sstream>
#include <fstream>

namespace FMDATA
{

using namespace std;
VGrid::VGrid(int nx,int ny,int nz)
{
	init(nx,ny,nz);
}

void VGrid::init(int nx,int ny,int nz)
{
	m_nx = nx;
	m_ny = ny;
	m_nz = nz;
	m_grid.clear();
	m_grid.reserve(nx*ny*nz);
}

void VGrid::setdata(int ix,int iy,int iz,float data)
{
	long index = ix+iy*m_nx+iz*m_nx*m_ny;
	if(ix>=0 && ix<m_nx && iy>=0 && iy<m_ny && iz>=0 && iz<m_nz)
		m_grid.at(index) = data;
}

float VGrid::Data(int ix,int iy,int iz) const
{
	long index = ix+iy*m_nx+iz*m_nx*m_ny;
	if(ix>=0 && ix<m_nx && iy>=0 && iy<m_ny && iz>=0 && iz<m_nz)
		return m_grid.at(index);
	else
		return INVALID_DATA;
}

bool VGrid::append(float data)
{
	if (m_grid.size()<size())
	{
		//m_grid.at(i) = data;
		m_grid.push_back(data);
		return true;
	}
	return false;
}

float VGrid::Data(int index) const
{
	if (index>=0 && index<size())
	{
		return m_grid.at(index);
	}
	else
	{
		return INVALID_DATA;
	}
}

long VGrid::size() const
{
	return m_nx*m_ny*m_nz;
}

void VGrid::SetDomain(VGridPara vpar)
{
	m_fxmin = R2D*vpar.f21;
	m_fymin = R2D*vpar.f22;
	m_fzmin = (EARTH_RADIUS-vpar.f23);
	m_fxmax = R2D*(vpar.f21 + vpar.f1*vpar.nx);
	m_fymax = R2D*(vpar.f22 + vpar.f2*vpar.ny);
	m_fzmax = m_fzmin + vpar.f3*vpar.nz;
}

void VGrid::SetDomain(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax)
{
   m_fxmin = xmin;
   m_fxmax = xmax;
   m_fymin = ymin;
   m_fymax = ymax;
   m_fzmin = zmin;
   m_fzmax = zmax;
}

vector<ProfileData> VGrid::GetProfile(int indexx,int indexY,int indexZ)
{
	//////////////////////////////////////////////////////////////////////////
	vector<ProfileData> profiledatavector;
	//init profile xy,初始化横剖面
	if (indexx>=0 && indexY>=0 && indexx<m_nx && indexY<m_ny)
	{
		ProfileData data;
		data.n_u = m_nx;
		data.n_v = m_ny;
		data.umin = 114.100;
		data.umax = 119.500;
		data.vmin = 37.819;
		data.vmax = 43.151;

		for (int i=0;i<m_nx;i++)
		{
			for (int j=0;j<m_ny;j++)
			{
				data.m_data.push_back(Data(i,j,indexZ));
			}
		}
		profiledatavector.push_back(data);
	}

	return profiledatavector;

}


bool VGrid::GetProfile(ProfileData& data,string profiletype,float fSliceValue)
{
	int indexofSlice = 0;
	//横剖面
	if (profiletype=="DEPTH")
	{
		//ProfileData data;
		data.n_u = m_nx;
		data.n_v = m_ny;
		data.umin = m_fxmin;
		data.umax = m_fxmax;
		data.vmin = m_fymin;
		data.vmax = m_fymax;

		indexofSlice = m_nz*(fSliceValue-m_fzmin)/(m_fzmax-m_fzmin);

		for (int i=0;i<m_nx;i++)
		{
			for (int j=0;j<m_ny;j++)
			{
				data.m_data.push_back(Data(i,j,indexofSlice));
			}
		}

		return true;
	}
	else if (profiletype=="EW") //东西剖面
	{
		//ProfileData data;
		data.n_u = m_nx;
		data.n_v = m_nz;
		data.umin = m_fxmin;
		data.umax = m_fxmax;
		data.vmin = m_fzmin;
		data.vmax = m_fzmax;

		indexofSlice = m_ny*(fSliceValue-m_fymin)/(m_fymax-m_fymin);

		for (int i=0;i<m_nx;i++)
		{
			for (int j=0;j<m_nz;j++)
			{
				data.m_data.push_back(Data(i,indexofSlice,j));
			}
		}

		return true;
	}
	else if (profiletype=="NS")//南北剖面
	{
		//ProfileData data;
		data.n_u = m_ny;
		data.n_v = m_nz;
		data.umin = m_fymin;
		data.umax = m_fymax;
		data.vmin = m_fzmin;
		data.vmax = m_fzmax;

		indexofSlice = m_nx*(fSliceValue-m_fxmin)/(m_fxmax-m_fxmin);

		for (int i=0;i<m_ny;i++)
		{
			for (int j=0;j<m_nz;j++)
			{
				data.m_data.push_back(Data(indexofSlice,i,j));
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////

VGrids::VGrids(const char* filename)
{

}

VGrids::~VGrids()
{

}

bool VGrids::readVgridData(const char* filename)
{
	FILE* fp;
	if ((fp=fopen(filename,"r"))==NULL)
	{
		return false;
	}
	fscanf(fp,"%d %d\n",&m_nBlock,&m_nmiss);
	if (m_nBlock>0)
	{
		for (int i=0;i<m_nBlock;i++)
		{
			VGridPara GridPara;
			fscanf(fp,"%d %d %d\n",&GridPara.nz,&GridPara.ny,&GridPara.nx);
			fscanf(fp,"%f %f %f\n",&GridPara.f3,&GridPara.f2,&GridPara.f1);
		    fscanf(fp,"%f %f %f\n",&GridPara.f23,&GridPara.f22,&GridPara.f21);

			int Gridsize = GridPara.nx*GridPara.ny*GridPara.nz;
			if(Gridsize ==0)
				return false;

			VGrid grid = VGrid(GridPara.nx,GridPara.ny,GridPara.nz);
			grid.SetDomain(GridPara);

			int nCount = 0;
			float fvalue;
			while(nCount<Gridsize && !feof(fp))
			{
				fscanf(fp,"%f",&fvalue);
				grid.append(fvalue);
				nCount++;
			}

			m_vGridVector.push_back(grid);
		}
	}
	/*
	std::ifstream fin(filename, std::ios::in); 
	char line[1024]={0}; 

	fin.getline(line, sizeof(line));
	std::stringstream word(line); 
	word>>m_nBlock;
	word>>m_nmiss;

	if (m_nBlock>0)
	{
		for (int i=0;i<m_nBlock;i++)
		{
			VGridPara GridPara;
			fin.getline(line, sizeof(line));
			std::stringstream word(line); 
			word>>GridPara.nz>>GridPara.ny>>GridPara.nx;

			fin.getline(line, sizeof(line));
			std::stringstream word2(line);
			word2>>GridPara.f3>>GridPara.f2>>GridPara.f1;

			fin.getline(line, sizeof(line));
			std::stringstream word3(line);
			word3>>GridPara.f23>>GridPara.f22>>GridPara.f21;
			
			int Gridsize = GridPara.nx*GridPara.ny*GridPara.nz;
			if(Gridsize ==0)
				return false;

			VGrid grid = VGrid(GridPara.nx,GridPara.ny,GridPara.nz);
	        grid.SetDomain(GridPara);

			int nCount = 0;
			float fvalue;
			while(nCount<Gridsize && fin.getline(line, sizeof(line)))
			{
				std::stringstream words(line); 
				words>>fvalue;
				grid.append(fvalue);
				nCount++;
			}

			m_vGridVector.push_back(grid);
		}
	}
	else
	{
		return false;
	}
	*/
	return true;
}

bool VGrids::readVgridRef(const char* filename)
{

	return true;
}

VGrid* VGrids::GetVGrid(int index)
 {
	 if (index>=0 && index<m_vGridVector.size())
	 {
		 return &m_vGridVector.at(index);
	 }
	 else
		 return NULL;
 }
}

//////////////////////////////////////////////////////////////////////////

