////////////////////////////////////////////////////////////////////////////////////////////////////
// Get data from earth textures.                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
class CTexture
{
public:
    void    Resolve( const char *pSurfaceFilename, const char *pOceanFilename );
    void    Save( const char *pFilename );
    
private:

    static const uint8_t TYPE_NOT_ASSIGN = 0;
    static const uint8_t TYPE_SURFACE = 1;
    static const uint8_t TYPE_WATER = 2;
    static const uint8_t TYPE_UNKNOWN = 3;

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
        
        uint8_t value;
        uint8_t type;
    };
    
    void    Load( const char *pSurfaceFilename, const char *pOceanFilename );
    void    CreateBuffer();
    void    ResolveData();
    void    LoadFileToBuffer( const char *pFilename, SBuffer *pBuffer );
    
    std::vector< SData >    m_buffer;
    SBuffer                 m_bufferS; // Surface
    SBuffer                 m_bufferO; // Ocean
};
////////////////////////////////////////////////////////////////////////////////////////////////////
