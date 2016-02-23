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
            char *buffer;

            input >> bufferSize;

            buffer = new char[ bufferSize ];

            auto bytesRead = input.ReadBytes( buffer, bufferSize );

            UAssert( bytesRead == bufferSize,
                "Mismatch buffer sizes. Expected %zd bytes got %zd",
                bufferSize,
                bytesRead
            );

            dx::TexMetadata meta;
            dx::ScratchImage image;

            auto result = LoadFromDDSMemory( 
                buffer, 
                bufferSize, 
                dx::DDS_FLAGS_NONE, 
                &meta, 
                image 
            );

            UAssert( result == S_OK,
                "Unable to load DDS from memory."
            );

            delete[] buffer;

            return std::make_shared<TextureData>(
                image.GetPixels( ), 
                image.GetPixelsSize( ), 
                static_cast<unsigned>( meta.width ), 
                static_cast<unsigned>( meta.height ) 
            );
        }
    }
}