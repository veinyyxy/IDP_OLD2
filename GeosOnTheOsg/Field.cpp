#include "Field.h"

GOTO_NAMESPACE_START

GeomField::GeomField( std::string name, int type, std::string typeName, int len, int prec, std::string comment )
	: mName( name ), mType( type ), mTypeName( typeName ),
	mLength( len ), mPrecision( prec ), mComment( comment )
{
}


GeomField::~GeomField()
{
}

bool GeomField::operator==( const GeomField& other ) const
{
	return (( mName == other.mName ) && ( mType == other.mType ) && ( mTypeName == other.mTypeName )
		&& ( mLength == other.mLength ) && ( mPrecision == other.mPrecision ) );
}

const std::string & GeomField::name() const
{
	return mName;
}

int GeomField::type() const
{
	return mType;
}

const std::string & GeomField::typeName() const
{
	return mTypeName;
}

int GeomField::length() const
{
	return mLength;
}

int GeomField::precision() const
{
	return mPrecision;
}

const std::string & GeomField::comment() const
{
	return mComment;
}

void GeomField::setName( const std::string & nam )
{
	mName = nam;
}

void GeomField::setType( int type )
{
	mType = type;
}

void GeomField::setTypeName( const std::string & typeName )
{
	mTypeName = typeName;
}

void GeomField::setLength( int len )
{
	mLength = len;
}

void GeomField::setPrecision( int prec )
{
	mPrecision = prec;
}

void GeomField::setComment( const std::string & comment )
{
	mComment = comment;
}

void GeomField::setX( double x )
{
	m_x = x;
}

void GeomField::setY( double y )
{
	m_y = y;
}

void GeomField::setZ( double z )
{
	m_z = z;
}

double GeomField::x()
{
	return m_x;
}

double GeomField::y()
{
	return m_y;
}

double GeomField::z()
{
	return m_z;
}

GOTO_NAMESPACE_END