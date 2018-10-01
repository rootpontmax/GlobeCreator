#include <iostream>
#include <cmath>

#include "Texture.h"
#include "Calculator.h"
#include "Utils.h"

#include "Icosahedron.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
struct SGlobePoint
{
    float posX;
    float posY;
    float posZ;
    
    float norX;
    float norY;
    float norZ;
    
    uint8_t color[4];
};
////////////////////////////////////////////////////////////////////////////////////////////////////
const static float EARTH_RAD = 6380.0e3f;
const static float SMALL_RAD = 1000.0f;
////////////////////////////////////////////////////////////////////////////////////////////////////
const static float RAD = 6380.0e3f;
const static float A_TO_R_COEF = 0.9510565163f;
const static float CONST_PI = 3.14159f;
////////////////////////////////////////////////////////////////////////////////////////////////////
static float GetIcosahedronArcAngle()
{
    const float c = 2.0f * A_TO_R_COEF * A_TO_R_COEF;
    const float cosinus = ( c - 1 ) / c;
    const float angleRad = acos( cosinus );
    const float angleDeg = angleRad * 180.0f / CONST_PI;
    return angleDeg;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static const float GetEdgeLength( const float angle )
{
    assert( angle >= 0.0f && angle <= 360.0f );
    const float equator = 2.0f * CONST_PI * RAD; 
    const float length = equator * angle / 360.0f;
    return length;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static void CalcAndReportMaxPossibleSingleGlobe()
{
    std::cout << "Max sigle patch globe" << std::endl;
    CCalculator calculator( RAD );
    calculator.CreateMaxPossibleSingleGlobe();
    calculator.Report();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static void CalcAndReportMaxPatch()
{
    int triaCount = 1;
    int vertCount = 3;
    int edgeCount = 3;
    int iterCount = 0;
    for( ; ; )
    {
        const int nextT = 4 * triaCount;
        const int nextV = vertCount + edgeCount;
        const int nextE = 2 * edgeCount + 3 * triaCount;
        
        if( nextV > 65536 )
            break;
            
        triaCount = nextT;
        vertCount = nextV;
        edgeCount = nextE;
        
        ++iterCount;
    }
    
    std::cout << "Max patch after " << iterCount << " interation(s) consist of:" << std::endl;
    std::cout << "Triangles: " << triaCount << std::endl;
    std::cout << "Vertices:  " << vertCount << std::endl;
    std::cout << "Edged:     " << edgeCount << std::endl;
    std::cout << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static void CalcAndReportMaxIcosahedron( const int iterCount )
{
    const uint64_t limit32 = 0xFFFFFFFF;
    uint64_t triaCount = 20;
    uint64_t vertCount = 12;
    uint64_t edgeCount = 30;
    
    for( int i = 0; i < iterCount; ++i )
    {
        const uint64_t nextT = 4 * triaCount;
        const uint64_t nextV = vertCount + edgeCount;
        const uint64_t nextE = 2 * edgeCount + 3 * triaCount;
        
        triaCount = nextT;
        vertCount = nextV;
        edgeCount = nextE;
    }
    
    std::cout << "Whole globe after " << iterCount << " interation(s) consist of:" << std::endl;
    std::cout << "Triangles: " << triaCount << std::endl;
    std::cout << "Vertices:  " << vertCount << std::endl;
    std::cout << "Edged:     " << edgeCount << std::endl;
    
    //std::cout << "Vertices limit " << limit32 << std::endl;
    
    if( vertCount >= limit32 || triaCount >= limit32 || edgeCount >= limit32 )
        std::cout << "32-bit limit exceeded" << std::endl; 
    std::cout << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static void CreateGeoData()
{
    CTexture texture;
    texture.Resolve( "Data/SurfaceHeight.jpeg", "Data/OceanDepth.jpeg" );
    texture.Save( "Data/TypeData.bin" );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static void CreateIcosahedron( const int iterationCount )
{
    CIcosahedron icosahedron;
    
    icosahedron.Create( iterationCount );
    icosahedron.Report();
    icosahedron.Save( "Data/VertData.bin", "Data/GridData.bin" );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    const int ITERATION_COUNT = 8;
    std::cout << "Create data for planet model" << std::endl;
    
    //CCalculator calculator( EARTH_RAD );
    CCalculator calculator( SMALL_RAD );
    calculator.CalcResolution( ITERATION_COUNT );
    
    //CreateGeoData();
    CreateIcosahedron( ITERATION_COUNT );
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
