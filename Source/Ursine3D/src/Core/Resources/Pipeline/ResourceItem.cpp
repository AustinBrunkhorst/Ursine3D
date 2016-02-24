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

    const meta::Type &rp::ResourceItem::GetDataType(void) const
    {
        return m_buildCache.processedType;
    }

    const fs::path &rp::ResourceItem::GetSourceFileName(void) const
    {
        return m_fileName;
    }

    const fs::path &rp::ResourceItem::GetBuildFileName(void) const
    {
        return m_buildFileName;
    }

    bool rp::ResourceItem::HasPreview(void) const
    {
        return m_buildCache.hasPreview;
    }

    const fs::path &rp::ResourceItem::GetPreviewFileName(void) const
    {
        return m_buildPreviewFileName;
    }

    std::string rp::ResourceItem::GetDisplayName(void) const
    {
        return change_extension( m_fileName.filename( ), "" ).string( );
    }
}