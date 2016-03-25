#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "TextDataStructures.h"

class TextLoadingException
{
public:
    TextLoadingException() { m_exception = "Not provided"; }
    TextLoadingException(std::string exception) { m_exception = exception; }

    void Speak(void) { std::cout << m_exception << std::endl; }
private:
    std::string m_exception;
};

class BitmapFont
{
public:
    BitmapFont(void);

    void Load(const std::string textPath);
    void Load(const uint8_t *binaryData, size_t size);

    // acessor stuff for rendering
    const std::vector<std::string> &GetTextureFiles(void) const;

    const std::vector<CharacterData> &GetCharacterData(void) const;

    const CommonData &GetCommonData(void) const;

    const InfoData &GetInfoData(void) const;

    const std::vector<std::string> &GetPageData(void) const;

private:
    template<typename T>
    T ReadData(int size)
    {
        if(m_binaryData == nullptr)
        {
            if ( m_input.is_open() )
            {
                char buffer[ 16 ];
                memset(buffer, 0, sizeof(char) * 16);

                m_input.read(buffer, size);

                return *reinterpret_cast<T*>(buffer);
            }
        }
        else
        {
            // read the binary
            char buffer[ 16 ];

            memset(buffer, 0, sizeof(char) * 16);

            memcpy(buffer, m_binaryData + m_position, sizeof(char) * size);

            m_position += size;

            return *reinterpret_cast<T*>(buffer);
        }

        return T();
    }

    // special case
    template<>
    int16_t ReadData(int size)
    {
        if (m_binaryData == nullptr)
        {
            if ( m_input.is_open() )
            {
                char buffer[ 16 ];
                memset(buffer, 0, sizeof(char) * 16);

                m_input.read(buffer, size);

                return buffer[ 0 ] | buffer[ 1 ] << 8;
            }
        }
        else
        {
            // read the binary
            char buffer[ 16 ];

            memset(buffer, 0, sizeof(char) * 16);

            memcpy(buffer, m_binaryData + m_position, sizeof(char) * size);

            m_position += size;

            return buffer[ 0 ] | buffer[ 1 ] << 8;
        }

        return 0;
    }

    void ReadBytes(void *destination, size_t count);

    bool ValidateFile(void);
    void ReadInfoData(int blockSize);
    void ReadCommonData(int blockSize);
    void ReadPageData(int blockSize);
    void ReadCharacterData(int blockSize);
    void ReadKerningData(int blockSize);

    unsigned GetBlockType(int &size);

    unsigned GetFileSize(const char* filename);

private:
    std::fstream                m_input;
    const uint8_t               *m_binaryData;
    size_t                      m_binarySize;
    size_t                      m_position;

    bool                        m_useBinary;

    InfoData                    m_infoData;
    CommonData                  m_commonData;
    std::vector<std::string>    m_pageData;
    std::vector<CharacterData>  m_characterData;
};