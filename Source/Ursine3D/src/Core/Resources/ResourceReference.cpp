#include "UrsinePrecompiled.h"

#include "ResourceReference.h"

#include "ResourceManager.h"

namespace ursine
{
    namespace resources
    {
        ResourceReference::ResourceReference(ResourceManager *manager, const GUID &resourceGUID)
            : m_manager( manager )
            , m_resourceGUID( resourceGUID ) { }
    }
}