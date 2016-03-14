#include "UrsinePrecompiled.h"

#include "ResourceItem.h"
#include "ResourcePipelineManager.h"

namespace ursine 
{
    rp::ResourceItem::ResourceItem(ResourcePipelineManager *manager, const GUID &guid)
        : m_isDirectoryResource( false )
        , m_manager( manager )
        , m_directoryNode( nullptr )
        , m_guid( guid )
    {
        // default to NULL
        m_buildCache.parent = GUIDNullGenerator( )( );
    }

    rp::ResourceItem::~ResourceItem(void) { }

    bool rp::ResourceItem::IsDirectoryResource(void) const
    {
        return m_isDirectoryResource;
    }

    rp::ResourceItem::Handle rp::ResourceItem::GetParent(void) const
    {
        return m_manager->GetItem( m_buildCache.parent );
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

    const fs::path &rp::ResourceItem::GetBuildCacheFileName(void) const
    {
        return m_buildCacheFileName;
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

    const rp::ResourceBuildCache &rp::ResourceItem::GetBuildCache(void) const
    {
        return m_buildCache;
    }
}