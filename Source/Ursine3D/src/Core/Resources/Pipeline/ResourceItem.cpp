#include "UrsinePrecompiled.h"

#include "ResourceItem.h"

namespace ursine 
{
    namespace resources
    {
        namespace pipeline
        {
            ResourceItem::ResourceItem(const GUID &guid)
                : m_guid( guid )
            {
                
            }

            const GUID &ResourceItem::GetGUID(void) const
            {
                return m_guid;
            }

            const fs::path& ResourceItem::GetBuildFileName(void) const
            {
                return m_buildFileName;
            }
        }
    }
}