#include "UrsinePrecompiled.h"

#include "ResourceItem.h"

namespace ursine 
{
    rp::ResourceItem::ResourceItem(const GUID &guid)
        : m_guid( guid )
    {
                
    }

    const GUID &rp::ResourceItem::GetGUID(void) const
    {
        return m_guid;
    }

    const fs::path &rp::ResourceItem::GetSourceFileName(void) const
    {
        return m_fileName;
    }

    const fs::path &rp::ResourceItem::GetBuildFileName(void) const
    {
        return m_buildFileName;
    }

    std::string resources::pipeline::ResourceItem::GetDisplayName(void) const
    {
        return change_extension( m_fileName.filename( ), "" ).string( );
    }
}