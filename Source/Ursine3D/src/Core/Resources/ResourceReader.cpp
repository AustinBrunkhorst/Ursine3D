#include "UrsinePrecompiled.h"

#include "ResourceReader.h"

namespace ursine 
{
    namespace resources
    {
        ResourceReader::ResourceReader(void)
            : m_stream( nullptr )
        {

        }

        ResourceReader::ResourceReader(const fs::path &file)
            : m_stream( std::make_shared<std::ifstream>( file.string( ).c_str( ), std::ios::in | std::ios::binary ) )
        {
            
        }

        ResourceReader::ResourceReader(const void *buffer, size_t size)
            : m_stream( std::make_shared<std::istringstream>( 
                    std::string(
                        reinterpret_cast<const char*>( buffer ), 
                        reinterpret_cast<const char*>( buffer ) + size
                    ) 
                ) 
            )
        {
            
        }

        char ResourceReader::ReadByte(void)
        {
            char byte;

            m_stream->read( &byte, 1 );

            return byte;
        }

        size_t ResourceReader::ReadBytes(void *input, size_t count)
        {
            m_stream->read( reinterpret_cast<char*>( input ), count );

            return m_stream->gcount( );
        }

        ResourceReader &ResourceReader::Read(std::string &input)
        {
            unsigned length;

            Read( length );

            input.resize( length );

            ReadBytes( &input[ 0 ], length );

            return *this;
        }

        ResourceReader &ResourceReader::Read(BinaryData &input)
        {
            uint64 size;

            Read( size );

            auto *data = new byte[ size ];

            auto bytesRead = ReadBytes( data, size );

            UAssertCatchable( bytesRead == size,
                "Mismatch buffer sizes. Expected %zd bytes got %zd",
                size,
                bytesRead
            );

            BinaryData::Load( input, data, size );

            return *this;
        }

        ResourceReader &ResourceReader::Seek(size_t offset)
        {
            m_stream->seekg( offset );

            return *this;
        }

        bool ResourceReader::IsOpen(void) const
        {
            return m_stream && !m_stream->fail( );
        }

        bool ResourceReader::IsEOF(void) const
        {
            return !m_stream || m_stream->eof( );
        }
    }
}