#pragma once

#include <osg/Image>
#include <osg/Notify>
#include <osg/Geode>
#include <osg/GL>

#include <osgDB/Registry>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/ImageOptions>
#include <osgDB/ReaderWriter>

#include <OpenThreads/ScopedLock>
#include <OpenThreads/ReentrantMutex>

#include <memory>

#include <gdal_priv.h>

#include "DataSetLayer.h"

#define SERIALIZER() OpenThreads::ScopedLock<OpenThreads::ReentrantMutex> lock(_serializerMutex)

//class ReadResult;

using namespace osgDB;

struct GeoRef 
{
	float flonmin;
	float flonmax;
	float flatmax;
	float flatmin;
//	float dx,dy;
	unsigned int m_width;
	unsigned int m_height;
};


class ReaderWriterGDAL : public osgDB::ReaderWriter
{
public:
	ReaderWriterGDAL();
public:
	virtual const char* className() const { return "GDAL Image Reader"; }
	virtual ReaderWriter::ReadResult readObject(const std::string& file, const osgDB::ReaderWriter::Options* options) const;
	virtual ReaderWriter::ReadResult readImage(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const;
	virtual ReaderWriter::ReadResult readHeightField(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const;
	
	virtual ReaderWriter::ReadResult local_readImage(const std::string& file, const osgDB::ReaderWriter::Options* options);
	ReaderWriter::ReadResult local_readHeightField(const std::string& fileName, const osgDB::ReaderWriter::Options* options);
	void initGDAL() const;
	//int width() {return m_width;}
	//int height() {return m_height;} 
	GeoRef  GetGeoRef() {return georef;}
private:
	mutable OpenThreads::ReentrantMutex _serializerMutex;	
	unsigned int m_width;
	unsigned int m_height;
	GeoRef georef;
};