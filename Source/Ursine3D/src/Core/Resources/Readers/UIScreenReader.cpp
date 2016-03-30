#include "UrsinePrecompiled.h"

#include "UIScreenReader.h"
#include "UIScreenData.h"

namespace ursine
{
    namespace resources
    {
        UIScreenReader::UIScreenReader(void) { }

        ResourceData::Handle UIScreenReader::Read(ResourceReader &input)
        {
            std::string jsonString;

            input.Read( jsonString );

            std::string jsonError;

            auto json = Json::parse( jsonString, jsonError );

            UAssertCatchable( jsonError.empty( ),
                "Error parsing screen JSON.\nerror: %s",
                jsonError.c_str( )
            );

            return std::make_shared<UIScreenData>( json );
        }
    }
}