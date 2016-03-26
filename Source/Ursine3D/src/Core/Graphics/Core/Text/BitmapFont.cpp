#include "UrsinePrecompiled.h"

#include "BitmapFont.h"
#include <iostream>

BitmapFont::BitmapFont(void)
    : m_input( )
    , m_binaryData( nullptr )
    , m_binarySize( 0 )
    , m_position( 0 )
    , m_useBinary( false )
    , m_infoData( )
    , m_commonData( )
    , m_pageData( )
    , m_characterData( )


{
}

void BitmapFont::Load(const std::string textPath)
{
    m_input.open(textPath, std::ios::binary | std::ios::in);

    if ( !m_input.is_open() )
    {
        std::cout << "Unable to open file." << std::endl;
        return;
    }
    std::cout << "Found '" << textPath << "'" << std::endl;
    
    // get size of the file
    unsigned sizeOfFile = GetFileSize(textPath.c_str());
    
    // make sure the file exists
    ValidateFile();

    // process the blocks
    do
    {
        int blockSize;
        switch ( GetBlockType(blockSize) )
        {
        case 1:
            ReadInfoData(blockSize);
            break;
        case 2:
            ReadCommonData(blockSize);
            break;
        case 3:
            ReadPageData(blockSize);
            break;
        case 4:
            ReadCharacterData(blockSize);
            break;
        case 5:
            ReadKerningData(blockSize);
            break;
        }
    } while ( !m_input.eof() );
}

void BitmapFont::Load(const uint8_t *binaryData, size_t size)
{
    m_useBinary = true;

    m_binaryData = binaryData;
    m_binarySize = size;
    m_position = 0;

    ValidateFile( ); 

    do
    {
        int blockSize;
        switch (GetBlockType(blockSize))
        {
        case 1:
            ReadInfoData(blockSize);
            break;
        case 2:
            ReadCommonData(blockSize);
            break;
        case 3:
            ReadPageData(blockSize);
            break;
        case 4:
            ReadCharacterData(blockSize);
            break;
        case 5:
            ReadKerningData(blockSize);
            break;
        }
    } while ( m_position < m_binarySize );
}

const std::vector<std::string>& BitmapFont::GetTextureFiles(void) const
{
    return m_pageData;
}

const std::vector<CharacterData>& BitmapFont::GetCharacterData(void) const
{
    return m_characterData;
}

const CommonData & BitmapFont::GetCommonData(void) const
{
    return m_commonData;
}

const InfoData & BitmapFont::GetInfoData(void) const
{
    return m_infoData;
}

const std::vector<std::string> &BitmapFont::GetPageData(void) const
{
    return m_pageData;
}

void BitmapFont::ReadBytes(void *destination, size_t count)
{
    if (m_binaryData == nullptr)
    {
        if (m_input.is_open( ))
            m_input.read( static_cast<char*>( destination ), count );
    }
    else
    {
        memcpy( destination, m_binaryData + m_position, count );

        m_position += count;
    }
}

bool BitmapFont::ValidateFile(void)
{
    char signature[4];

    signature[ 0 ] = ReadData<char>( 1 );
    signature[ 1 ] = ReadData<char>( 1 );
    signature[ 2 ] = ReadData<char>( 1 );
    signature[ 3 ] = 0;

    UAssertCatchable( strcmp( signature, "BMF" ) == 0,
        "Invalid .fnt file magic. "
        "Make sure the font is built with the binary descriptor."    
    );

    auto formatVersion = ReadData<int>( 1 );

    UAssertCatchable( formatVersion == 3,
        "Unknown .fnt file version. Expected '%i' got '%i'.",
        3, 
        formatVersion
    );

    return true;
}

void BitmapFont::ReadInfoData(int blockSize)
{
    m_infoData.fontSize                     = ReadData<int16_t>(2);
    m_infoData.bitField                     = ReadData<uint8_t>(1);
    m_infoData.characterSet                 = ReadData<uint8_t>(1);
    m_infoData.stretchHorizontal            = ReadData<uint16_t>(2);
    m_infoData.superSamplingLevel           = ReadData<uint8_t>(1);
    m_infoData.paddingUp                    = ReadData<uint8_t>(1);
    m_infoData.paddingRight                 = ReadData<uint8_t>(1);
    m_infoData.paddingDown                  = ReadData<uint8_t>(1);
    m_infoData.paddingLeft                  = ReadData<uint8_t>(1);
    m_infoData.spacing.spacingHorizontal    = ReadData<uint8_t>(1);
    m_infoData.spacing.spacingVertical      = ReadData<uint8_t>(1);
    m_infoData.outlineThickness             = ReadData<uint8_t>(1);

    auto stringLength = blockSize - 14;

    m_infoData.fontName.resize( stringLength );

    ReadBytes( &m_infoData.fontName[ 0 ], stringLength );
}

void BitmapFont::ReadCommonData(int blockSize)
{
    m_commonData.lineHeight                 = ReadData<uint16_t>(2);
    m_commonData.base                       = ReadData<uint16_t>(2);
    m_commonData.textureDimensions.scaleW   = static_cast<float>(ReadData<uint16_t>(2));
    m_commonData.textureDimensions.scaleH   = static_cast<float>(ReadData<uint16_t>(2));
    m_commonData.pages                      = ReadData<uint16_t>(2);
    m_commonData.bitField                   = ReadData<uint8_t>(1);
    m_commonData.colorChannels.alphaChannel = ReadData<uint8_t>(1);
    m_commonData.colorChannels.redChannel   = ReadData<uint8_t>(1);
    m_commonData.colorChannels.greenChannel = ReadData<uint8_t>(1);
    m_commonData.colorChannels.blueChannel  = ReadData<uint8_t>(1);
}

void BitmapFont::ReadPageData(int blockSize)
{
    // how many bytes will each string be?
    unsigned pageSize = blockSize / m_commonData.pages;

    char tempBuffer[ 128 ];

    // for each page
    for (unsigned x = 0; x < m_commonData.pages; ++x)
    {
        m_pageData.emplace_back( );

        auto &page = m_pageData.back( );

        page.resize( pageSize );

        ReadBytes( tempBuffer, pageSize );

        page = std::string(tempBuffer);
    }
}

void BitmapFont::ReadCharacterData(int blockSize)
{
    // size of each character data set is 20 bytes
    unsigned characterCount = blockSize / 20;

    // resize array
    m_characterData.resize(characterCount);

    // read each character
    for ( unsigned x = 0; x < characterCount; ++x )
    {
        uint32_t id = ReadData<uint32_t>(4);

        // allow us to keep the index as the ID
        if ( id >= m_characterData.size() )
            m_characterData.resize(id + 1);

        m_characterData[ id ].id        = id;
        float xPos                      = static_cast<float>(ReadData<uint32_t>(2));
        float yPos                      = static_cast<float>(ReadData<uint32_t>(2));
        float width                     = static_cast<float>(ReadData<uint32_t>(2));
        float height                    = static_cast<float>(ReadData<uint32_t>(2));
        m_characterData[ id ].offset.x  = ReadData<int16_t>(2);
        m_characterData[ id ].offset.y  = ReadData<int16_t>(2);
        m_characterData[ id ].xadvance  = ReadData<int16_t>(2);
        m_characterData[ id ].page      = ReadData<uint32_t>(1);
        m_characterData[ id ].chnl      = ReadData<uint32_t>(1);

        m_characterData[ id ].textureCoordinates.x = xPos;// / m_commonData.textureDimensions.scaleW;
        m_characterData[ id ].textureCoordinates.y = yPos;// / m_commonData.textureDimensions.scaleH;
                         
        m_characterData[ id ].textureDimensions.x = width;// / m_commonData.textureDimensions.scaleW;
        m_characterData[ id ].textureDimensions.y = height;// / m_commonData.textureDimensions.scaleH;
    }
}

void BitmapFont::ReadKerningData(int blockSize)
{
    // resize everything
    unsigned kerningCount = blockSize / 10;
        
    for ( unsigned x = 0; x < kerningCount; ++x )
    {
        unsigned first = ReadData<unsigned>(4);
        unsigned second = ReadData<unsigned>(4);
        
        m_characterData[ first ].kerningMap.insert(
            std::pair<unsigned, 
            int16_t>(
                second, 
                ReadData<int16_t>(2)
            )
        );
    }
}

unsigned BitmapFont::GetBlockType(int & size)
{
    int id = ReadData<int>(1);
    size = ReadData<int>(4);
    return id;
}

unsigned BitmapFont::GetFileSize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return static_cast<unsigned>(in.tellg());
}
