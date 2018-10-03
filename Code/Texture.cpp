#include "Texture.h"
#include <cassert>
#include <iostream>
#include <cstdint>

#include "jpeg/jpgd.h"
#include "jpeg/jpge.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
static const int g_bpp = 4;
CTexture::SLatLon::SLatLon() :
    x( 0 ),
    y( 0 )
{}
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
    height( 0.0f ),
    colR( 0 ),
    colG( 0 ),
    colB( 0 ),
    value( 0 ),
    type( TYPE_NOT_ASSIGN )
{}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CTexture::Resolve( const char *pSurfaceFilename, const char *pOceanFilename, const char *pColorFilename )
{
    Load( pSurfaceFilename, pOceanFilename, pColorFilename );
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
    const size_t sizeHeight = imageW * imageH;
    const size_t sizeColor = sizeHeight * 3;
    std::vector< uint8_t > tempBufferHeight;
    std::vector< uint8_t > tempBufferColor;
    tempBufferHeight.resize( sizeHeight );
    tempBufferColor.resize( sizeColor );
    
        
    for( size_t i = 0; i < sizeHeight; ++i )
    {
        //tempBufferHeight[i] = m_buffer[i].value;
        tempBufferHeight[i] = static_cast< uint8_t >( ( m_buffer[i].height + 1.0f ) * 0.5f * 255.0f );
        
        const size_t j = i * 3;
        tempBufferColor[j    ] = m_buffer[i].colR;
        tempBufferColor[j + 1] = m_buffer[i].colG;
        tempBufferColor[j + 2] = m_buffer[i].colB;
    }
        
    
    jpge::params outParamHeight;
    jpge::params outParamColor;
    outParamHeight.m_quality = 100;
    outParamHeight.m_subsampling = jpge::Y_ONLY;
    
    outParamColor.m_quality = 100;
    outParamColor.m_subsampling = jpge::H2V2;
    
    const int outChannelsHeight = 1;
    const int outChannelsColor = 3;
    const uint8_t *pOutHeight = &tempBufferHeight[0];
    const uint8_t *pOutColor = &tempBufferColor[0];
    
    bool res = true;
    res &= jpge::compress_image_to_jpeg_file( "Data/__Buffer.jpeg", imageW, imageH, outChannelsHeight, pOutHeight, outParamHeight );
    res &= jpge::compress_image_to_jpeg_file( "Data/__Colors.jpeg", imageW, imageH, outChannelsColor, pOutColor, outParamColor );
    assert( res );
    std::cout << "Save\n";
    // end of CRAP
}
////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CTexture::GetColor( const float angleLat, const float angleLon ) const
{
    const int sizeX = m_bufferS.imageW;
    const SLatLon coords = GetCoords( angleLat, angleLon );
    
    /*
    const int sizeX = m_bufferS.imageW;
    const int sizeY = m_bufferS.imageH;
    const float coefLat = ( angleLat + 90.0f ) / 180.0f;
    const float coefLon = angleLon / 360.0f;
    const int x = static_cast< int >( coefLon * ( sizeX - 1 ) );
    const int y = ( m_bufferS.imageH - 1 ) - static_cast< int >( coefLat * ( sizeY - 1 ) );
    
    assert( x >= 0 && x < sizeX );
    assert( y >= 0 && y < sizeY );
    
    const size_t offset = y * sizeX + x;
    */
    
    const size_t offset = coords.y * sizeX + coords.x;
    
    /*
    const uint8_t col = m_buffer[offset].value;
    
    const uint32_t colR = col;
    const uint32_t colG = col << 8;
    const uint32_t colB = col << 16;
    */
    
    const uint32_t colR = m_buffer[offset].colR;
    const uint32_t colG = m_buffer[offset].colG << 8;
    const uint32_t colB = m_buffer[offset].colB << 16;
    
    const uint32_t color = 0xFF000000 |colB | colG | colR;
    
    //.....................0xAABBGGRR
    //const uint32_t color = 0x00FF0000;
    
    return color;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
float CTexture::GetHeight( const float angleLat, const float angleLon ) const
{
    const int sizeX = m_bufferS.imageW;
    const SLatLon coords = GetCoords( angleLat, angleLon );
    const size_t offset = coords.y * sizeX + coords.x;
    
    return m_buffer[offset].height;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CTexture::Load( const char *pSurfaceFilename, const char *pOceanFilename, const char *pColorFilename )
{
    LoadFileToBuffer( pSurfaceFilename, &m_bufferS );
    LoadFileToBuffer( pOceanFilename, &m_bufferO );
    LoadFileToBuffer( pColorFilename, &m_bufferC );
    
    
    assert( m_bufferS.pBuffer );
    assert( m_bufferO.pBuffer );
    assert( m_bufferC.pBuffer );
    assert( m_bufferS.imageW != 0 && m_bufferS.imageW ==  m_bufferO.imageW && m_bufferS.imageW == m_bufferC.imageW );
    assert( m_bufferS.imageH != 0 && m_bufferS.imageH ==  m_bufferO.imageH && m_bufferS.imageH == m_bufferC.imageH );
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
            const size_t offsetC = offset * g_bpp;
            
            const uint8_t colS = m_bufferS.pBuffer[offsetS];
            const uint8_t colO = m_bufferO.pBuffer[offsetO];
            
            const uint8_t colR = m_bufferC.pBuffer[offsetC];
            const uint8_t colG = m_bufferC.pBuffer[offsetC + 1];
            const uint8_t colB = m_bufferC.pBuffer[offsetC + 2];
            
            m_buffer[offset].colR = colR;
            m_buffer[offset].colG = colG;
            m_buffer[offset].colB = colB;
            
            if( colS == colO )
            {
                assert( colS >= 0xF7 );
                m_buffer[offset].type = TYPE_UNKNOWN;
                ++unknownCount;
                
                // CRAP - поставим тип Surface с нулевой глубиной
                m_buffer[offset].height = 0.0f;
                m_buffer[offset].value = 0x00;
                m_buffer[offset].type = TYPE_SURFACE;
                // end of CRAP
                
                // CRAP - тестирование конфликтных пикселей
                //m_buffer[offset].value = 0x00;
                // end of CRAP
            }
            
            else if( colS == 0xFF )
            {
                m_buffer[offset].value = colO;
                m_buffer[offset].type = TYPE_WATER;
                m_buffer[offset].height = -( static_cast< float >( colO ) / 255.0f );
            }
            else if( colO == 0xFF )
            {
                m_buffer[offset].value = colS;
                m_buffer[offset].type = TYPE_SURFACE;
                m_buffer[offset].height = static_cast< float >( colS ) / 255.0f;
                
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
CTexture::SLatLon CTexture::GetCoords( const float angleLat, const float angleLon ) const
{
    const int sizeX = m_bufferS.imageW;
    const int sizeY = m_bufferS.imageH;
    const float coefLat = ( angleLat + 90.0f ) / 180.0f;
    const float coefLon = angleLon / 360.0f;
    
    const int x = static_cast< int >( coefLon * ( sizeX - 1 ) );
    const int y = ( m_bufferS.imageH - 1 ) - static_cast< int >( coefLat * ( sizeY - 1 ) );
    
    assert( x >= 0 && x < sizeX );
    assert( y >= 0 && y < sizeY );
    
    SLatLon ret;
    ret.x = x;
    ret.y = y;
    return ret;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

