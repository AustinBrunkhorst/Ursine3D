#include "UrsinePrecompiled.h"

#include "ResourceItem.h"
#include "ResourcePipelineManager.h"

namespace ursine 
{
    rp::ResourceItem::ResourceItem(ResourcePipelineManager *manager, const GUID &guid)
        : m_manager( manager )
        , m_parent( nullptr )
        , m_guid( guid ) { }

    rp::ResourceItem::~ResourceItem(void) { }

    rp::ResourceItem::Handle rp::ResourceItem::GetParent(void) const
    {
        return m_parent;
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
        return m_fileName.filename( ).stem( ).string( );
    }

    std::string rp::ResourceItem::GetRelativePathDisplayName(void) const
    {
        auto relative = fs::MakeRelativePath(
            m_manager->GetConfig( ).resourceDirectory, 
            m_fileName 
        );

        return change_extension( relative, "" ).string( );
    }
}