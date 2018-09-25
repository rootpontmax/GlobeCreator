////////////////////////////////////////////////////////////////////////////////////////////////////
// Aux functions that used for some icosahedron calculations.                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////////////////////////
class CCalculator
{
public:
    CCalculator( const float radius );
    
    void CreateMaxPossibleSingleGlobe();
    void CreateMaxPossibleSinglePatch();
    void Report();
    
    
    
    
private:

    struct SData
    {
        SData();
        SData( const uint64_t tria, const uint64_t vert, const uint64_t edge );
        
        uint64_t triaCount;
        uint64_t vertCount;
        uint64_t edgeCount;
    };
    
    void    SplitTillVertexLimit16();
    SData   SplitData( const SData& data ) const;
    
    SData           m_data;
    int             m_iterCount;
    const float     m_radius;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
