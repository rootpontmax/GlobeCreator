////////////////////////////////////////////////////////////////////////////////////////////////////
// Icosahedron class with patch partition.                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#define USE_HASH_FOR_ICOSAHEDRON
#define USE_PACKED_SAVE

#include "Types.h"
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>



////////////////////////////////////////////////////////////////////////////////////////////////////
class CIcosahedron
{
public:
    void    Create( const int levelCount );
    void    Report();
    void    Save( const char *pVertFilename, const char *pGridFilename );

private:

    // Объявления некоторых типов
#ifdef USE_HASH_FOR_ICOSAHEDRON
    typedef std::unordered_map< SEdge, int, SEdgeHasher >   TMap;
    typedef std::unordered_set< SEdge, SEdgeHasher >        TSet;
#else
    typedef std::map< SEdge, int >  TMap;
    typedef std::set< SEdge >       TSet;
#endif

    
    
    // Функции создания
    void    CreateInitial();
    void    Split( const int startEdgeID, const int startFaceID );
    
    // Вспомогательные функции
    void    EstablishConnectivity( const int startEdgeID, const int startFaceID );
    void    FixVertOrderInFace( SFace *pFace );
    int     FindEdgeID( const std::map< SEdge, int >& dict, const SEdge& edge );
    int     FindEdgeID( const SEdge& edge );
    
    
    // Основное хранилище
    std::vector< SVert >    m_vert;
    std::vector< SEdge >    m_edge;
    std::vector< SFace >    m_face;
    
    // Вспомогательное хранилище
    TMap                    m_edgeMap;
    TSet                    m_edgeSet;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
