////////////////////////////////////////////////////////////////////////////////////////////////////
// Get data from earth textures.                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
class CTexture
{
public:
    void        Resolve( const char *pSurfaceFilename, const char *pOceanFilename, const char *pColorFilename );
    void        Save( const char *pFilename );
    uint32_t    GetColor( const float angleLat, const float angleLon ) const;
    float       GetHeight( const float angleLat, const float angleLon ) const;
    
private:

    static const uint8_t TYPE_NOT_ASSIGN = 0;
    static const uint8_t TYPE_SURFACE = 1;
    static const uint8_t TYPE_WATER = 2;
    static const uint8_t TYPE_UNKNOWN = 3;
    
    struct SLatLon
    {
        SLatLon();
        
        int x;
        int y;
    };

    struct SBuffer
    {
        SBuffer();
        ~SBuffer();
        
        const uint8_t  *pBuffer;
        int             imageW;
        int             imageH;
        int             bpp;
    };
    
    struct SData
    {
        SData();
        
        float   height;
        uint8_t colR;
        uint8_t colG;
        uint8_t colB;
        uint8_t valueS;
        uint8_t valueO;
        uint8_t type;
    };
    
    void    Load( const char *pSurfaceFilename, const char *pOceanFilename, const char *pColorFilename );
    void    CreateBuffer();
    void    ResolveData();
    void    LoadFileToBuffer( const char *pFilename, SBuffer *pBuffer );
    SLatLon GetCoords( const float angleLat, const float angleLon ) const;
    
    std::vector< SData >    m_buffer;
    SBuffer                 m_bufferS; // Surface
    SBuffer                 m_bufferO; // Ocean
    SBuffer                 m_bufferC; // Color
};
////////////////////////////////////////////////////////////////////////////////////////////////////
