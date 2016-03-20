#include "UrsinePrecompiled.h"

#include "UIScreenProcessor.h"

#include "JsonData.h"
#include "UIScreenData.h"

namespace ursine
{
    rp::UIScreenProcessor::UIScreenProcessor(void) { }

    resources::ResourceData::Handle rp::UIScreenProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        auto *json = expectInput<JsonData>( input );

        auto &data = json->GetData( );

        auto &projectData = data[ "project" ];
        auto &pathData = data[ "path" ];

        UAssertCatchable( projectData.is_string( ) && pathData.is_string( ),
            "Invalid JSON screen structure."    
        );

        return std::make_shared<UIScreenData>(
            GUIDStringGenerator( )( projectData.string_value( ) ),
            pathData.string_value( )
        );
    }
}