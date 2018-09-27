////////////////////////////////////////////////////////////////////////////////////////////////////
// Icosahedron class with patch partition.                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Types.h"
#include <map>

////////////////////////////////////////////////////////////////////////////////////////////////////
class CIcosahedron
{
public:
    void    Create( const int levelCount );
    void    Report();
    void    Save( const char *pFilename );

private:
    // Функции создания
    void    CreateInitial();
    void    Split( const int startEdgeID, const int startFaceID );
    
    // Вспомогательные функции
    void    EstablishConnectivity( const int startEdgeID, const int startFaceID );
    void    FixVertOrderInFace( SFace *pFace );
    int     FindEdgeID( const std::map< SEdge, int >& dict, const SEdge& edge );
    
    std::vector< SVert >    m_vert;
    std::vector< SEdge >    m_edge;
    std::vector< SFace >    m_face;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
