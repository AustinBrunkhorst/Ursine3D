/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TextureReader.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
            BinaryData data;

            input.Read( data );

            dx::TexMetadata meta;

            auto result = GetMetadataFromDDSMemory( 
                data.GetData( ), 
                data.GetSize( ), 
                dx::DDS_FLAGS_NONE, 
                meta 
            );

            UAssertCatchable( result == S_OK,
                "Unable to load DDS from memory."
            );

            return std::make_shared<TextureData>(
                std::move( data ),
                static_cast<unsigned>( meta.width ), 
                static_cast<unsigned>( meta.height ) 
            );
        }
    }
}