#pragma once

#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
static const int INVALID_ID = -1;
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SFace;
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SVert
{
    explicit SVert();
    explicit SVert( const float _x, const float _y, const float _z );
    SVert& operator+( const SVert& rhs );
    SVert& operator*( float coef );
    
    SVert   GetNormalazed() const;
    void    Normalize();
    
    float       x;
    float       y;
    float       z;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
SVert operator+( const SVert& lhs, const SVert& rhs );
SVert operator-( const SVert& lhs, const SVert& rhs );
float Dot( const SVert& lhs, const SVert& rhs );
SVert Cross( const SVert& lhs, const SVert& rhs );
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SEdge
{
    explicit SEdge();
    explicit SEdge( const int _idA, const int _idB );
    
    void    RegisterFace( const int id );
    int     GetNeighbour( const int thisID );

    int         idA;        // Original point
    int         idB;        // Original point
    int         idC;        // Middle point
    int         faceID[2];  // Face
};
////////////////////////////////////////////////////////////////////////////////////////////////////
bool operator<( const SEdge& lhs, const SEdge& rhs );
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SFace
{
    explicit SFace( const int idPointA, const int idPointB, const int idPointC );

    int         parentID;       // Parent face id
    int         pointID[3];     // Point id
    int         edgeID[3];      // Edge id
    int         neighbourID[3]; // Same-level neighbour face id
    int         childID[4];     // Child face id
};
////////////////////////////////////////////////////////////////////////////////////////////////////
