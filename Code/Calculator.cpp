#include "Calculator.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
const static float A_TO_R_COEF = 0.9510565163f;
const static float CONST_PI = 3.14159f;
////////////////////////////////////////////////////////////////////////////////////////////////////
CCalculator::SData::SData() :
    triaCount( 20 ),
    vertCount( 12 ),
    edgeCount( 30 )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
CCalculator::SData::SData( const uint64_t tria, const uint64_t vert, const uint64_t edge ) :
    triaCount( tria ),
    vertCount( vert ),
    edgeCount( edge )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
CCalculator::CCalculator( const float radius ) :
    m_iterCount( 0 ),
    m_radius( radius )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalculator::CreateMaxPossibleSingleGlobe()
{
    SData data;
    m_data = data;
    SplitTillVertexLimit16();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalculator::CreateMaxPossibleSinglePatch()
{
    SData data();
    m_data = data;
    SplitTillVertexLimit16();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalculator::Report()
{
    std::cout << "Max sigle patch globe after " << m_iterCount << " interation(s) consist of:" << std::endl;
    std::cout << "\tTriangles: " << m_data.triaCount << std::endl;
    std::cout << "\tVertices:  " << m_data.vertCount << std::endl;
    std::cout << "\tEdges:     " << m_data.edgeCount << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalculator::SplitTillVertexLimit16()
{
    m_iterCount = 0;
    for( ; ; )
    {
        SData splitData = SplitData( m_data );
        
        if( splitData.vertCount > 65536 )
            break;
            
        m_data = std::move( splitData );
        ++m_iterCount;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
CCalculator::SData CCalculator::SplitData( const SData& data ) const
{
    const uint64_t nextT = 4 * data.triaCount;
    const uint64_t nextV = data.vertCount + data.edgeCount;
    const uint64_t nextE = 2 * data.edgeCount + 3 * data.triaCount;
        
    SData retData( nextT, nextV, nextE );
    return retData;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
