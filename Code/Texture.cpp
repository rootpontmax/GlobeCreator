#include "Texture.h"
#include <cassert>
#include <iostream>

#include "jpeg/jpgd.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
CTexture::SBuffer::SBuffer() :
    pBuffer( nullptr ),
    imageW( 0 ),
    imageH( 0 ),
    bpp( 0 )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
CTexture::SBuffer::~SBuffer()
{
    if( pBuffer )
        free( (void*)pBuffer );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
CTexture::SData::SData() :
    value( 0 ),
    type( TYPE_NOT_ASSIGN )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CTexture::Resolve( const char *pSurfaceFilename, const char *pOceanFilename )
{
    Load( pSurfaceFilename, pOceanFilename );
    CreateBuffer();
    ResolveData();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CTexture::Save( const char *pFilename )
{
    // TODO: Implement it
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CTexture::Load( const char *pSurfaceFilename, const char *pOceanFilename )
{
    LoadFileToBuffer( pSurfaceFilename, &m_bufferS );
    LoadFileToBuffer( pOceanFilename, &m_bufferO );
    
    assert( m_bufferS.pBuffer );
    assert( m_bufferO.pBuffer );
    assert( m_bufferS.imageW != 0 && m_bufferS.imageW ==  m_bufferO.imageW );
    assert( m_bufferS.imageH != 0 && m_bufferS.imageH ==  m_bufferO.imageH );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CTexture::CreateBuffer()
{
    const size_t size = m_bufferS.imageW * m_bufferS.imageH; 
    m_buffer.resize( size );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CTexture::ResolveData()
{
    const int imageW = m_bufferS.imageW;
    const int imageH = m_bufferS.imageH;
    const int bppS = m_bufferS.bpp;
    const int bppO = m_bufferO.bpp;
    
    int totalCount = 0;
    int unknownCount = 0;
    
    for( int y = 0; y < imageH; ++y )
        for( int x = 0; x < imageW; ++x )
        {
            const int offset = ( y * imageW + x );
            const int offsetS = offset * bppS;
            const int offsetO = offset * bppO;
            
            const uint8_t colS = m_bufferS.pBuffer[offsetS];
            const uint8_t colO = m_bufferO.pBuffer[offsetO];
            
            if( colS == colO )
            {
                assert( colS >= 0xF7 );
                m_buffer[offset].type = TYPE_UNKNOWN;
                ++unknownCount;
                
                // CRAP - поставим тип Surface с нулевой глубиной
                m_buffer[offset].value = 0x00;
                m_buffer[offset].type = TYPE_SURFACE;
                // end of CRAP
            }
            
            else if( colS == 0xFF )
            {
                m_buffer[offset].value = colO;
                m_buffer[offset].type = TYPE_WATER;
            }
            else if( colO == 0xFF )
            {
                m_buffer[offset].value = colS;
                m_buffer[offset].type = TYPE_SURFACE;
            }
            ++totalCount;
        }
        
    // Report
    const float unknownPercent = static_cast< float >( unknownCount ) / static_cast< float >( totalCount ) * 100.0f;
    std::cout << "Unknown pixel percent: " << unknownPercent << std::endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CTexture::LoadFileToBuffer( const char *pFilename, SBuffer *pBuffer )
{
    assert( pFilename );
    assert( pBuffer );
    
    const int reqComps = 4;
    pBuffer->pBuffer = jpgd::decompress_jpeg_image_from_file( pFilename,
                                                              &pBuffer->imageW,
                                                              &pBuffer->imageH,
                                                              &pBuffer->bpp,
                                                              reqComps );
}
////////////////////////////////////////////////////////////////////////////////////////////////////

