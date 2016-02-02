#include "Resource.h"
#include "ResourceImportOptions.h"

namespace ursine
{
    template<typename OptionsType>
    OptionsType *ResourceImportContext::GetImportOptions(void)
    {
        static_assert( std::is_base_of<ResourceImportOptions, OptionsType>::value, 
            "The specified type does not derive from ResourceImportOptions." 
        );

        auto *options = m_resource->GetImportOptions( );

        auto optionsType = typeof( OptionsType );

        UAssert( optionsType.IsValid( ),
            "Invalid options type (is it enabled in reflection?).\nWith OptionsType = '%s'.",
            typeid( OptionsType ).name( )
        );

        UAssert( options->GetType( ) == optionsType,
            "Invalid import options cast.\nExpected '%s' but got '%s'.",
            optionsType.GetName( ).c_str( ),
            options->GetType( ).GetName( ).c_str( )
        );

        return static_cast<OptionsType *>( options );
    }
}