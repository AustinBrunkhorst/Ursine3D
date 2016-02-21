#include "UrsinePrecompiled.h"

#include "JsonData.h"
#include "JsonReader.h"

namespace ursine
{
    namespace resources
    {
        void JsonData::Write(pipeline::ResourceWriter &output)
        {
            output << "PoopyTexture";
        }

        meta::Type JsonData::GetReaderType(void)
        {
            return typeof( JsonReader );
        }
    }
}