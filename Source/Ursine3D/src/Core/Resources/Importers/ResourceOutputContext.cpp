#include "UrsinePrecompiled.h"

#include "ResourceOutputContext.h"

#include "Resource.h"

namespace ursine
{
    ResourceOutputContext::ResourceOutputContext(Resource *resource)
        : m_resource( resource ) { }

    std::fstream ResourceOutputContext::GetOutputFileHandle(void) const
    {
        return std::fstream( m_resource->GetOutputPath( ).string( ) );
    }
}