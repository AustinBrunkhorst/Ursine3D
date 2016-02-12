#include "BitmapFont.h"
#include <iostream>

BitmapFont::BitmapFont(const std::string textPath)
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

const std::vector<std::string>& BitmapFont::GetTextureFiles(void) const
{
    return m_pageData;
}

bool BitmapFont::ValidateFile(void)
{
	char signature[4];

	signature[0] = ReadData<char>(1);
	signature[1] = ReadData<char>(1);
	signature[2] = ReadData<char>(1);
	signature[3] = 0;

	if(strcmp(signature, "BMF") != 0)
	{
        throw TextLoadingException("File is not a valid .fnt file!");
        return false;
	}

    int formatVersion = ReadData<int>(1);

    if ( formatVersion != 3 )
    {
        throw TextLoadingException("File is an older version of .fnt, upgrade software!");
        return false;
    }

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

    int stringsize = blockSize - 14;
    char stringBuff[ 512 ];
    m_input.read(stringBuff, stringsize);
    m_infoData.fontName = std::string(stringBuff);
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

    // for each page
    for ( unsigned x = 0; x < m_commonData.pages; ++x )
    {
        char tempBuffer[ 1024 ];

        // read in data
        m_input.read(tempBuffer, pageSize);

        // add to page vector
        m_pageData.push_back(tempBuffer);
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

        m_characterData[ id ].textureCoordinates.x = xPos / m_commonData.textureDimensions.scaleW;
        m_characterData[ id ].textureCoordinates.y = yPos / m_commonData.textureDimensions.scaleH;
                         
        m_characterData[ id ].textureDimensions.x = width / m_commonData.textureDimensions.scaleW;
        m_characterData[ id ].textureDimensions.y = height / m_commonData.textureDimensions.scaleH;
    }
}

void BitmapFont::ReadKerningData(int blockSize)
{
    // resize everything
    m_kerningPairs.resize(m_characterData.size());
    for ( auto &x : m_kerningPairs )
        x.resize(m_characterData.size());

    unsigned kerningCount = blockSize / 10;

    for ( unsigned x = 0; x < kerningCount; ++x )
    {
        unsigned first = ReadData<unsigned>(4);
        unsigned second = ReadData<unsigned>(4);

        m_characterData[ first ].kerningMap.insert(
            std::pair<unsigned, 
            int16_t>(second, ReadData<int16_t>(2))
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
