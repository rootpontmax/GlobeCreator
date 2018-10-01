#include "Icosahedron.h"
#include <iostream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "Calculator.h"
#include "Utils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::Create( const int levelCount )
{
    std::cout<<"Creation..." << std::endl;
    
    const uint64_t creationStartTimeMS = GetProcessTimeMS();
    
    // Calculate size of all buffers and preallocate memory for that
    CCalculator calculator( 1.0f );
    calculator.CreateHierarcyGlobe( levelCount );
    m_vert.reserve( calculator.GetVertCount() );
    m_edge.reserve( calculator.GetEdgeCount() );
    m_face.reserve( calculator.GetTriaCount() );
    
#ifdef USE_HASH_FOR_ICOSAHEDRON
    m_edgeMap.reserve( calculator.GetMaxEdgeCount() );
    m_edgeSet.reserve( calculator.GetMaxEdgeCount() );
#endif
    
    // Construct initial globe
    CreateInitial();
    
    // Split it and create hierarchy
    int edgeStartID = 0;
    int faceStartID = 0;
    for( int i = 0; i < levelCount; ++i )
    {
        const int nextEdgeStartID = static_cast< int >( m_edge.size() );
        const int nextFaceStartID = static_cast< int >( m_face.size() );
        
        Split( edgeStartID, faceStartID );
        
        edgeStartID = nextEdgeStartID;
        faceStartID = nextFaceStartID;
    }
    
    // Check the time
    const uint64_t deltaTime = GetProcessTimeMS() - creationStartTimeMS;
    std::cout<<"\tDone for " << deltaTime << " ms." << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::Report()
{
    std::cout << std::endl;
    std::cout<<"Icosahedron:" << std::endl;
    std::cout<<"\tVertex count: " << m_vert.size() << std::endl;
    std::cout<<"\tEdge count:   " << m_edge.size() << std::endl;
    std::cout<<"\tFace count:   " << m_face.size() << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::Save( const char *pVertFilename, const char *pGridFilename )
{
    std::cout << std::endl;
    std::cout<<"Saving..." << std::endl;
    
    const uint64_t savingStartTimeMS = GetProcessTimeMS();
    
    const int vertCount = static_cast< int >( m_vert.size() );
    const int faceCount = static_cast< int >( m_face.size() );
    
    std::ofstream fileVert;
    std::ofstream fileGrid;
    fileVert.open( pVertFilename, std::ios::out | std::ios::binary );
    fileGrid.open( pGridFilename, std::ios::out | std::ios::binary );
    
    // Write header
    fileVert.write( (char*)&vertCount, sizeof( int ) );
    
    fileGrid.write( (char*)&vertCount, sizeof( int ) );
    fileGrid.write( (char*)&faceCount, sizeof( int ) );
    
    // Save vertices information
    for( int i = 0; i < vertCount; ++i )
    {
        const SVert& vert = m_vert[i];
        fileVert.write( (char*)&vert.x, sizeof( float ) );
        fileVert.write( (char*)&vert.y, sizeof( float ) );
        fileVert.write( (char*)&vert.z, sizeof( float ) );
    }
    
    // Save faces information
    for( int i = 0; i < faceCount; ++i )
    {
        const SFace& face = m_face[i];
        
#ifdef USE_PACKED_SAVE
    SaveInt24( fileGrid, face.parentID );
        
    for( int j = 0; j < 3; ++j )
    {
        SaveInt24( fileGrid, face.pointID[j] );
        SaveInt24( fileGrid, face.neighbourID[j] );
    }
    for( int j = 0; j < 4; ++j )
        SaveInt24( fileGrid, face.childID[j] );
#else
    fileGrid.write( (char*)&face.parentID, sizeof( int ) );
    for( int j = 0; j < 3; ++j )
    {
        fileGrid.write( (char*)&face.pointID[j], sizeof( int ) );
        fileGrid.write( (char*)&face.neighbourID[j], sizeof( int ) );
    }
    for( int j = 0; j < 4; ++j )
        fileGrid.write( (char*)&face.childID[j], sizeof( int ) );
#endif
    }
    
    fileVert.close();
    fileGrid.close();
    
    // Check the time
    const uint64_t deltaTime = GetProcessTimeMS() - savingStartTimeMS;
    std::cout<<"\tDone for " << deltaTime << " ms." << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::CreateInitial()
{
    const float A = 0.26286500f;
    const float B = 0.42532500f;
    const float C = 0.0f;

    // Vertices
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
    m_vert.push_back( SVert( -B, -A,  C ) ); // 11
    assert( m_vert.size() == 12 );
    
    // Normalize every vertex
    for( size_t i = 0; i < m_vert.size(); ++i )
        m_vert[i].Normalize();
    
    // Trianlges. Create and assign regionID
    m_face.push_back( SFace( 0,  6, 1 ) );
    m_face.push_back( SFace( 0, 11, 6 ) );
    m_face.push_back( SFace( 1,  4, 0 ) );
    m_face.push_back( SFace( 1,  8, 4 ) );
    m_face.push_back( SFace( 1, 10, 8 ) );
    m_face.push_back( SFace( 2,  5, 3 ) );
    m_face.push_back( SFace( 2,  9, 5 ) );
    m_face.push_back( SFace( 2, 11, 9 ) );
    m_face.push_back( SFace( 3,  7, 2 ) );
    m_face.push_back( SFace( 3, 10, 7 ) );
    m_face.push_back( SFace( 4,  8, 5 ) );
    m_face.push_back( SFace( 4,  9, 0 ) );
    m_face.push_back( SFace( 5,  8, 3 ) );
    m_face.push_back( SFace( 5,  9, 4 ) );
    m_face.push_back( SFace( 6, 10, 1 ) );
    m_face.push_back( SFace( 6, 11, 7 ) );
    m_face.push_back( SFace( 7, 10, 6 ) );
    m_face.push_back( SFace( 7, 11, 2 ) );
    m_face.push_back( SFace( 8, 10, 3 ) );
    m_face.push_back( SFace( 9, 11, 0 ) );
    assert( m_face.size() == 20 );
    
    // Fix vertices order in every face
    for( size_t i = 0; i < m_face.size(); ++i )
        FixVertOrderInFace( &m_face[i] );
        
    // Assign regionID and create edges
    m_edgeSet.clear();
    for( size_t i = 0; i < m_face.size(); ++i )
    {
        SFace& face = m_face[i]; 
        for( int i = 0; i < 3; ++i )
        {
            const int j = ( i + 1 ) % 3;
            const int idA = face.pointID[i];
            const int idB = face.pointID[j];
            const SEdge edge( idA, idB );
            m_edgeSet.insert( edge );
        }
    }
    
    // Add all edges to vector
    for( TSet::iterator it = m_edgeSet.begin(); it != m_edgeSet.end(); ++it )
    {
        const SEdge& edge = *it;
        m_edge.push_back( edge );
    }
    assert( m_edge.size() == 30 );
    
    EstablishConnectivity( 0, 0 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::Split( const int startEdgeID, const int startFaceID )
{
    const int vertCount = static_cast< int >( m_vert.size() );
    const int edgeCount = static_cast< int >( m_edge.size() );
    const int faceCount = static_cast< int >( m_face.size() );
    
    // For all egdes create middle point and add it to store
    for( int i = startEdgeID; i < edgeCount; ++i )
    {
        const int idA = m_edge[i].idA;
        const int idB = m_edge[i].idB;
        assert( idA >= 0 && idA < vertCount );
        assert( idB >= 0 && idB < vertCount );
        SVert middle = m_vert[idA] + m_vert[idB];
        middle.Normalize();
        
        const int idC = static_cast< int >( m_vert.size() );
        m_vert.push_back( middle );
        m_edge[i].idC = idC; 
    }
    
    // For all faces add edges
    m_edgeSet.clear();
    for( int i = startFaceID; i < faceCount; ++i )
    {
        // Create edges
        SFace& face = m_face[i];
        const int edgeIDA = face.edgeID[0];
        const int edgeIDB = face.edgeID[1];
        const int edgeIDC = face.edgeID[2];
        assert( edgeIDA >= 0 && edgeIDA < edgeCount );
        assert( edgeIDB >= 0 && edgeIDB < edgeCount );
        assert( edgeIDC >= 0 && edgeIDC < edgeCount );
        
        const SEdge& edgeA = m_edge[edgeIDA]; 
        const SEdge& edgeB = m_edge[edgeIDB];
        const SEdge& edgeC = m_edge[edgeIDC];
        
        // Create new edges taking in accout
        // Point M is a middle of AB
        // Point N is a middle of BC
        // Point O is a middle of CA
        m_edgeSet.insert( SEdge( edgeA.idA, edgeA.idC ) );
        m_edgeSet.insert( SEdge( edgeA.idB, edgeA.idC ) );
        
        m_edgeSet.insert( SEdge( edgeB.idA, edgeB.idC ) );
        m_edgeSet.insert( SEdge( edgeB.idB, edgeB.idC ) );
        
        m_edgeSet.insert( SEdge( edgeC.idA, edgeC.idC ) );
        m_edgeSet.insert( SEdge( edgeC.idB, edgeC.idC ) );
        
        m_edgeSet.insert( SEdge( edgeA.idC, edgeB.idC ) );
        m_edgeSet.insert( SEdge( edgeB.idC, edgeC.idC ) );
        m_edgeSet.insert( SEdge( edgeC.idC, edgeA.idC ) );
        
        // Create four new faces and establish parent-child relationship
        SFace faceA( face.pointID[0], edgeA.idC, edgeC.idC );
        SFace faceB( face.pointID[1], edgeA.idC, edgeB.idC );
        SFace faceC( face.pointID[2], edgeB.idC, edgeC.idC );
        SFace faceD( edgeA.idC, edgeB.idC, edgeC.idC );
        
        const int newFaceID = static_cast< int >( m_face.size() );
        FixVertOrderInFace( &faceA );
        FixVertOrderInFace( &faceB );
        FixVertOrderInFace( &faceC );
        FixVertOrderInFace( &faceD );
        faceA.parentID = i;
        faceB.parentID = i;
        faceC.parentID = i;
        faceD.parentID = i;
        
        face.childID[0] = newFaceID;
        face.childID[1] = newFaceID + 1;
        face.childID[2] = newFaceID + 2;
        face.childID[3] = newFaceID + 3;
        
        m_face.push_back( faceA );
        m_face.push_back( faceB );
        m_face.push_back( faceC );
        m_face.push_back( faceD );
    }
    
    // Add edges from dict to main edge storage
    for( TSet::iterator it = m_edgeSet.begin(); it != m_edgeSet.end(); ++it )
    {
        const SEdge& edge = *it;
        m_edge.push_back( edge );
    }

    EstablishConnectivity( edgeCount, faceCount );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::EstablishConnectivity( const int startEdgeID, const int startFaceID )
{
    const int vertCount = static_cast< int >( m_vert.size() );
    const int faceCount = static_cast< int >( m_face.size() );
    const int edgeCount = static_cast< int >( m_edge.size() );
    
    // Create dictionary edge-id
    //std::map< SEdge, int > dict;
    //std::unordered_map< SEdge, int, SEdgeHasher > dict;
    m_edgeMap.clear();
    for( int i = startEdgeID; i < edgeCount; ++i )
        m_edgeMap.insert( std::pair< SEdge, int >( m_edge[i], i ) );
    
    // Create connectivity
    for( int faceID = startFaceID; faceID < faceCount; ++faceID )
        for( int i = 0; i < 3; ++i )
        {
            const int j = ( i + 1 ) % 3;
            const int idA = m_face[faceID].pointID[i];
            const int idB = m_face[faceID].pointID[j];
            assert( idA >= 0 && idA < vertCount );
            assert( idB >= 0 && idB < vertCount );
            const SEdge edge( idA, idB );
            
            const int edgeID = FindEdgeID( edge );
            assert( edgeID >= 0 && edgeID < edgeCount );
            m_face[faceID].edgeID[i] = edgeID;
            m_edge[edgeID].RegisterFace( faceID );
        }
        
    // Find neigbour for every face
    for( int faceID = startFaceID; faceID < faceCount; ++faceID )
        for( int i = 0; i < 3; ++i )
        {
            const int edgeID = m_face[faceID].edgeID[i];
            assert( edgeID >= 0 && edgeID < edgeCount );
            const int neighbourID = m_edge[edgeID].GetNeighbour( faceID );
            assert( neighbourID != faceID );
            assert( neighbourID >= 0 && neighbourID < faceCount );
            m_face[faceID].neighbourID[i] = neighbourID;
        }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CIcosahedron::FixVertOrderInFace( SFace *pFace )
{
    assert( pFace );
    
    const SVert& vertA = m_vert[pFace->pointID[0]];
    const SVert& vertB = m_vert[pFace->pointID[1]];
    const SVert& vertC = m_vert[pFace->pointID[2]];
    
    const SVert vertAB = vertB - vertA;
    const SVert vertAC = vertC - vertA;
    const SVert vertNor = Cross( vertAB, vertAC );
    const float dot = Dot( vertNor, vertA );
    
    if( dot < 0.0f )
        std::swap( pFace->pointID[0], pFace->pointID[1] );
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
int CIcosahedron::FindEdgeID( const SEdge& edge )
{
    TMap::const_iterator it = m_edgeMap.find( edge );
    if( it == m_edgeMap.end() )
    {
        assert( false );
        return INVALID_ID;
    }
    
    return it->second;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
