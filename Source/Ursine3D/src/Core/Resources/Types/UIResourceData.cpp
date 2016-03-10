#include "UrsinePrecompiled.h"

#include "UIResourceData.h"
#include "UIResourceReader.h"

namespace ursine
{
    namespace resources
    {
        UIResourceData::UIResourceData(void)
        {
            
        }

        void UIResourceData::Write(pipeline::ResourceWriter &output)
        {
            
        }

        meta::Type UIResourceData::GetReaderType(void)
        {
            return typeof( UIResourceReader );
        }
    }
}