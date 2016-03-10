#include "UrsinePrecompiled.h"

#include "FontReader.h"
#include "FontData.h"

#include <DirectXTex.h>

namespace dx = DirectX;

namespace ursine
{
    namespace resources
    {
        FontReader::FontReader(void) { }

        ResourceData::Handle FontReader::Read(ResourceReader &input)
        {
            return nullptr;
        }
    }
}