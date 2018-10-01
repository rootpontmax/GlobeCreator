#include "Texture.h"
#include <cassert>
#include <iostream>
#include <cstdint>

#include "jpeg/jpgd.h"
#include "jpeg/jpge.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
static const int g_bpp = 4;
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
    
    
    // CRAP - save file and look at conflicted pixels
    const int imageW = m_bufferS.imageW;
    const int imageH = m_bufferS.imageH;
    const size_t size = imageW * imageH;
    std::vector< uint8_t > tempBuffer;
    tempBuffer.resize( size );
    /*
    for( size_t y = 0; y < imageH; ++y )
        for( size_t x = 0; x < imageW; ++x )
        {
            const size_t offset = y * imageW + x;            
            tempBuffer[offset] = m_buffer[offset].value;
        }
        */
        
        
    for( size_t i = 0; i < size; ++i )
        tempBuffer[i] = m_buffer[i].value;
        
    
    jpge::params outParam;
    outParam.m_quality = 100;
    outParam.m_subsampling = jpge::Y_ONLY;
    const int outChannels = 1;
    const uint8_t *pOut = &tempBuffer[0];
    
    const bool res = jpge::compress_image_to_jpeg_file( "Data/Conflicted.jpeg", imageW, imageH, outChannels, pOut, outParam );
    assert( res );
    std::cout << "Save\n";
    // end of CRAP
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
    
    int totalCount = 0;
    int unknownCount = 0;
    
    for( int y = 0; y < imageH; ++y )
        for( int x = 0; x < imageW; ++x )
        {
            const size_t offset = y * imageW + x;
            const size_t offsetS = offset * g_bpp;
            const size_t offsetO = offset * g_bpp;
            
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
                
                // CRAP - тестирование конфликтных пикселей
                //m_buffer[offset].value = 0x00;
                // end of CRAP
            }
            
            else if( colS == 0xFF )
            {
                //m_buffer[offset].value = colO;
                //m_buffer[offset].type = TYPE_WATER;
                
                // CRAP - тестирование конфликтных пикселей
                //m_buffer[offset].value = 0xFF;
                // end of CRAP
            }
            else if( colO == 0xFF )
            {
                m_buffer[offset].value = colS;
                m_buffer[offset].type = TYPE_SURFACE;
                
                // CRAP - тестирование конфликтных пикселей
                //m_buffer[offset].value = 0xFF;
                // end of CRAP
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
    
    pBuffer->pBuffer = jpgd::decompress_jpeg_image_from_file( pFilename,
                                                              &pBuffer->imageW,
                                                              &pBuffer->imageH,
                                                              &pBuffer->bpp,
                                                              g_bpp );
}
////////////////////////////////////////////////////////////////////////////////////////////////////

