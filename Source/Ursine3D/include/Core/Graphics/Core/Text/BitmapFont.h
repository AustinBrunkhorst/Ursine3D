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
	BitmapFont(const std::string textPath);

    // acessor stuff for rendering
    const std::vector<std::string> &GetTextureFiles(void) const;

private:
	template<typename T>
	T ReadData(int size)
	{
		if(m_input.is_open())
		{
            char buffer[ 16 ];
            memset(buffer, 0, sizeof(char) * 16);

			m_input.read(buffer, size);

			return *reinterpret_cast<T*>(buffer);
		}

		return T();
	}

    // special case
    template<>
    int16_t ReadData(int size)
    {
        if ( m_input.is_open() )
        {
            char buffer[ 16 ];
            memset(buffer, 0, sizeof(char) * 16);

            m_input.read(buffer, size);

            return buffer[ 0 ] | buffer[ 1 ] << 8;
        }

        return 0;
    }

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

    InfoData                    m_infoData;
    CommonData                  m_commonData;
    std::vector<std::string>    m_pageData;
    std::vector<CharacterData>  m_characterData;

    // first index is first, second index is second
    std::vector<std::vector<int>> m_kerningPairs;
};