#include "Types.h"

#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////
SVert::SVert() :
    x( 0.0f ),
    y( 0.0f ),
    z( 0.0f )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
SVert::SVert( const float _x, const float _y, const float _z ) :
    x( _x ),
    y( _y ),
    z( _z )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
SVert& SVert::operator+( const SVert& rhs )
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SVert& SVert::operator*( float coef )
{
    x *= coef;
    y *= coef;
    z *= coef;
    return *this;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SVert SVert::GetNormalazed() const
{
    SVert res( x, y, z );
    res.Normalize();
    return res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void SVert::Normalize()
{
    const float length = sqrt( x * x + y * y + z * z );
    x /= length;
    y /= length;
    z /= length;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SVert operator+( const SVert& lhs, const SVert& rhs )
{
    SVert ret;
    ret.x = lhs.x + rhs.x;
    ret.y = lhs.y + rhs.y;
    ret.z = lhs.z + rhs.z;
    return ret;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SVert operator-( const SVert& lhs, const SVert& rhs )
{
    SVert ret;
    ret.x = lhs.x - rhs.x;
    ret.y = lhs.y - rhs.y;
    ret.z = lhs.z - rhs.z;
    return ret;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
float Dot( const SVert& lhs, const SVert& rhs )
{
    const float ret = lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    return ret; 
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SVert Cross( const SVert& lhs, const SVert& rhs )
{
    SVert ret;
    ret.x = lhs.y * rhs.z - lhs.z * rhs.y;
    ret.y = lhs.z * rhs.x - lhs.x * rhs.z;
    ret.z = lhs.x * rhs.y - lhs.y * rhs.x;
    return ret;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SEdge::SEdge() :
    idA( INVALID_ID ),
    idB( INVALID_ID ),
    faceID{ INVALID_ID, INVALID_ID }
{}
////////////////////////////////////////////////////////////////////////////////////////////////////    
SEdge::SEdge( const int _idA, const int _idB ) :
    faceID{ INVALID_ID, INVALID_ID }
{
    const bool bIsABigger = ( _idA > _idB ); 
    idA = bIsABigger ? _idB : _idA; 
    idB = bIsABigger ? _idA : _idB;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void SEdge::RegisterFace( const int id )
{
    for( int i = 0; i < 2; ++i )
        if( INVALID_ID == faceID[i] )
        {
            faceID[i] = id;
            return;
        }
    abort();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int SEdge::GetNeighbour( const int thisID )
{
    if( thisID == faceID[0] )
        return faceID[1];
    else if( thisID == faceID[1] )
        return faceID[0];
    else
        return INVALID_ID;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool operator<( const SEdge& lhs, const SEdge& rhs )
{
    if( lhs.idA < rhs.idA )
        return true;
    else if( lhs.idA == rhs.idA )
        return ( lhs.idB < rhs.idB );
    else
        return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
SFace::SFace( const int idPointA, const int idPointB, const int idPointC ) :
    parentID( INVALID_ID ),
    pointID{ idPointA, idPointB, idPointC },
    neighbourID{ INVALID_ID, INVALID_ID, INVALID_ID },
    childID{ INVALID_ID, INVALID_ID, INVALID_ID, INVALID_ID }
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
