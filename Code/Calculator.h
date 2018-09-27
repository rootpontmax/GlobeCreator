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
    
    void CreateHierarcyGlobe( const int iterationCount );
    
    // Get data
    int GetTriaCount() const { return m_data.triaCount; }
    int GetVertCount() const { return m_data.vertCount; }
    int GetEdgeCount() const { return m_data.edgeCount; }
    int GetMaxEdgeCount() const { return m_maxEdgeCount; }
    
    
    
    
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
    void    ReportData( const SData& data ) const;
    
    SData           m_data;
    uint64_t        m_maxEdgeCount;
    int             m_iterCount;
    const float     m_radius;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
