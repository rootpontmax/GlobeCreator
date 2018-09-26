#include "Icosahedron.h"
#include <set>

////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::Create( const int levelCount )
{
    // Calculate size of all buffers
    
    CreateInitial();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::Report()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::Save( const char *pFilename )
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::CreateInitial()
{
    const float A = 0.26286500f;
    const float B = 0.42532500f;
    const float C = 0.0f;

    // Vertexes
    m_vert.push_back( SVert( -A,  C,  B ) ); //  0
    m_vert.push_back( SVert(  A,  C,  B ) ); //  1
    m_vert.push_back( SVert( -A,  C, -B ) ); //  2
    m_vert.push_back( SVert(  A,  C, -B ) ); //  3
    m_vert.push_back( SVert(  C,  B,  A ) ); //  4
    m_vert.push_back( SVert(  C,  B, -A ) ); //  5
    m_vert.push_back( SVert(  C, -B,  A ) ); //  6
    m_vert.push_back( SVert(  C, -B, -A ) ); //  7
    m_vert.push_back( SVert(  B,  A,  C ) ); //  8
    m_vert.push_back( SVert( -B,  A,  C ) ); //  9
    m_vert.push_back( SVert(  B, -A,  C ) ); // 10
    m_vert.push_back( SVert( -B, -A,  C ) ); // 12
    
    // Trianlges. Create and assign regionID
    int regionID = 0;
    m_face.push_back( SFace( regionID++, 0,  6, 1 ) );
    m_face.push_back( SFace( regionID++, 0, 11, 6 ) );
    m_face.push_back( SFace( regionID++, 1,  4, 0 ) );
    m_face.push_back( SFace( regionID++, 1,  8, 4 ) );
    m_face.push_back( SFace( regionID++, 1, 10, 8 ) );
    m_face.push_back( SFace( regionID++, 2,  5, 3 ) );
    m_face.push_back( SFace( regionID++, 2,  9, 5 ) );
    m_face.push_back( SFace( regionID++, 2, 11, 9 ) );
    m_face.push_back( SFace( regionID++, 3,  7, 2 ) );
    m_face.push_back( SFace( regionID++, 3, 10, 7 ) );
    m_face.push_back( SFace( regionID++, 4,  8, 5 ) );
    m_face.push_back( SFace( regionID++, 4,  9, 0 ) );
    m_face.push_back( SFace( regionID++, 5,  8, 3 ) );
    m_face.push_back( SFace( regionID++, 5,  9, 4 ) );
    m_face.push_back( SFace( regionID++, 6, 10, 1 ) );
    m_face.push_back( SFace( regionID++, 6, 11, 7 ) );
    m_face.push_back( SFace( regionID++, 7, 10, 6 ) );
    m_face.push_back( SFace( regionID++, 7, 11, 2 ) );
    m_face.push_back( SFace( regionID++, 8, 10, 3 ) );
    m_face.push_back( SFace( regionID++, 9, 11, 0 ) );
    
    // Assign regionID and create edges
    std::set< SEdge > edgeDict;
    for( size_t i = 0; i < m_face.size(); ++i )
    {
        SFace& face = m_face[i]; 
        for( int i = 0; i < 3; ++i )
        {
            const int j = ( i + 1 ) % 3;
            const int idA = face.pointID[i];
            const int idB = face.pointID[j];
            const SEdge edge( idA, idB );
            edgeDict.insert( edge );
        }
    }
    
    // Copy all edges to vector
    for( std::set< SEdge >::iterator it = edgeDict.begin(); it != edgeDict.end(); ++it )
    {
        const SEdge& edge = *it;
        m_edge.push_back( edge );
    }
    assert( m_edge.size() == 30 );
    
    EstablishConnectivity();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::EstablishConnectivity()
{
    const int vertCount = static_cast< int >( m_vert.size() );
    const int faceCount = static_cast< int >( m_face.size() );
    const int edgeCount = static_cast< int >( m_edge.size() );
    
    // Create dictionary edge-id
    std::map< SEdge, int > dict;
    for( int i = 0; i < edgeCount; ++i )
        dict.insert( std::pair< SEdge, int >( m_edge[i], i ) );
    
    // Create connectivity
    for( int faceID = 0; faceID < faceCount; ++faceID )
        for( int i = 0; i < 3; ++i )
        {
            const int j = ( i + 1 ) % 3;
            const int idA = m_face[faceID].pointID[i];
            const int idB = m_face[faceID].pointID[j];
            assert( idA >= 0 && idA < vertCount );
            assert( idB >= 0 && idB < vertCount );
            const SEdge edge( idA, idB );
            const int edgeID = FindEdgeID( dict, edge );
            assert( edgeID >= 0 && edgeID < edgeCount );
            m_face[faceID].edgeID[i] = edgeID;
            m_edge[edgeID].RegisterFace( faceID );
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int CIcosahedron::FindEdgeID( const std::map< SEdge, int >& dict, const SEdge& edge )
{
    std::map< SEdge, int >::const_iterator it = dict.find( edge );
    if( it == dict.end() )
    {
        assert( false );
        return INVALID_ID;
    }
    
    return it->second;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
