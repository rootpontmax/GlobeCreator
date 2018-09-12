#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
const static float RAD = 6380.0e3f;
const static float A_TO_R_COEF = 0.9510565163f;
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
    int triaCount = 20;
    int vertCount = 12;
    int edgeCount = 30;
    
    for( int i = 0; i < iterCount; ++i )
    {
        const int nextT = 4 * triaCount;
        const int nextV = vertCount + edgeCount;
        const int nextE = 2 * edgeCount + 3 * triaCount;
        
        triaCount = nextT;
        vertCount = nextV;
        edgeCount = nextE;
    }
    
    std::cout << "Whole globe after " << iterCount << " interation(s) consist of:" << std::endl;
    std::cout << "Triangles: " << triaCount << std::endl;
    std::cout << "Vertices:  " << vertCount << std::endl;
    std::cout << "Edged:     " << edgeCount << std::endl;
    std::cout << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    std::cout<<"Icosahedron calculation" << std::endl;
    
    CalcAndReportMaxPatch();
    CalcAndReportMaxIcosahedron( 8 );
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
