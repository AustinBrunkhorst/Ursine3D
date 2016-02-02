#include "UrsinePrecompiled.h"

#include "ResourceImportContext.h"

#include "Resource.h"

namespace ursine
{
    ResourceImportContext::ResourceImportContext(Resource *resource)
        : m_resource( resource )
    {
        
    }

    std::ifstream ResourceImportContext::GetInputFileHandle(void) const
    {
        return std::ifstream( m_resource->GetAbsolutePath( ).string( ) );
    }
}