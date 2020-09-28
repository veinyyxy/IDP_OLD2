#pragma once
/** @file		Field.h
* @brief		
* @author		杨晓宇
* @date			2012.10
* @version		1.0
  @note			
*/

#include "goConfig.h"

GOTO_NAMESPACE_START

/** @class  GeomField
    @brief  图层或者是图元的属性描述
    @author 杨晓宇
    @note   看名字及可知道函数功能
*/
class GEOSONTHEOSG_EXPORT GeomField
{
 public:
    GeomField( std::string name = std::string(),
              int type = 0,
              std::string typeName = std::string(),
              int len = 0,
              int prec = 0,
              std::string comment = std::string() );
    ~GeomField();
    bool operator==( const GeomField& other ) const;
    const std::string & name() const;
    int type() const;
    const std::string & typeName() const;
    int length() const;
    int precision() const;
    const std::string & comment() const;
    void setName( const std::string & nam );
    void setType( int type );
    void setTypeName( const std::string & typ );
    void setLength( int len );
    void setPrecision( int prec );
    void setComment( const std::string & comment );

	void setX(double x);
	void setY(double y);
	void setZ(double z);
	double x();
	double y();
	double z();

  private:
    std::string mName;
    int mType;
    std::string mTypeName;
    int mLength;
    int mPrecision;
    std::string mComment;

	double m_x;
	double m_y;
	double m_z;
};

typedef std::map<int, GeomField>  FIELD_MAP, *P_FIELD_MAP;
typedef std::pair<int, GeomField>  FIELD_MAPPair, *P_FIELD_MAPPair;
GOTO_NAMESPACE_END