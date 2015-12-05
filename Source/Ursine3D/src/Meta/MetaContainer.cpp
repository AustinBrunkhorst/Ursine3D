#include "UrsinePrecompiled.h"

#include "MetaContainer.h"
#include "MetaManager.h"

namespace ursine
{
    namespace meta
    {
        const MetaManager &MetaContainer::GetMeta(void) const
        {
            return m_meta;
        }
    }
}