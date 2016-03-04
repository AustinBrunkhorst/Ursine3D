#include "UrsinePrecompiled.h"

#include "TextureReader.h"
#include "TextureData.h"

#include <DirectXTex.h>

namespace dx = DirectX;

namespace ursine
{
    namespace resources
    {
        TextureReader::TextureReader(void) { }

        ResourceData::Handle TextureReader::Read(ResourceReader &input)
        {
            size_t bufferSize;
            uint8 *buffer;

            input.Read( bufferSize );

            buffer = new uint8[ bufferSize ];

            auto bytesRead = input.ReadBytes( reinterpret_cast<char*>( buffer ), bufferSize );

            UAssertCatchable( bytesRead == bufferSize,
                "Mismatch buffer sizes. Expected %zd bytes got %zd",
                bufferSize,
                bytesRead
            );

            dx::TexMetadata meta;

            auto result = GetMetadataFromDDSMemory( 
                buffer, 
                bytesRead, 
                dx::DDS_FLAGS_NONE, 
                meta 
            );

            UAssertCatchable( result == S_OK,
                "Unable to load DDS from memory."
            );

            auto data = std::make_shared<TextureData>(
                buffer, 
                bytesRead, 
                static_cast<unsigned>( meta.width ), 
                static_cast<unsigned>( meta.height ) 
            );

            delete[] buffer;

            return data;
        }
    }
}