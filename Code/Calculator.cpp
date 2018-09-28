#include "Calculator.h"

#include <iostream>
#include <cmath>

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
    m_maxEdgeCount( 0 ),
    m_iterCount( 0 ),
    m_radius( radius )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalculator::CalcResolution( const int iterationCount )
{
    // Расчет угла оригинального икосаэдра
    const float c = 2.0f * A_TO_R_COEF * A_TO_R_COEF;
    const float cosinus = ( c - 1 ) / c;
    const float angleRad = acos( cosinus );
    float angleDeg = angleRad * 180.0f / CONST_PI;
    
    for( int i = 0; i < iterationCount; ++i )
        angleDeg /= 2.0f;
    
    const float equator = 2.0f * CONST_PI * m_radius; 
    const float edgeLength = equator * angleDeg / 360.0f;
    
    std::cout << "Edge angle after " << iterationCount << " iteration(s) is " <<
                angleDeg << "° and length is " << edgeLength << " meters" << std::endl;
    
    //angleDeg
}
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
    SData data;
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
void CCalculator::CreateHierarcyGlobe( const int iterationCount )
{
    SData data;
    uint64_t triaCount = data.triaCount;
    uint64_t edgeCount = data.edgeCount;
    
    for( int i = 0; i < iterationCount; ++i )
    {
        data = SplitData( data );
        triaCount += data.triaCount;
        edgeCount += data.edgeCount;
    }
    
    m_data = data;
    const uint64_t maxEdgeCount = m_data.edgeCount;
    m_data.edgeCount = edgeCount;
    m_data.triaCount = triaCount;
    
    std::cout << "Size of data after " << iterationCount << " interation(s) consist of:" << std::endl;
    std::cout << "\tVertices:    " << m_data.vertCount << std::endl;
    std::cout << "\tEdges (max): " << m_data.edgeCount << " (" << maxEdgeCount << ")" << std::endl;
    std::cout << "\tTriangles:   " << m_data.triaCount << std::endl;
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
void CCalculator::ReportData( const SData& data ) const
{
    std::cout << "Data report" << std::endl;
    std::cout << "\tVertices:  " << data.vertCount << std::endl;
    std::cout << "\tEdges:     " << data.edgeCount << std::endl;
    std::cout << "\tTriangles: " << data.triaCount << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
