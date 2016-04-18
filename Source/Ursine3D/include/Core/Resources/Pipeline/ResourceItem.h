/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceItem.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "GUID.h"

#include "ResourceMetaData.h"
#include "ResourceBuildCache.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourcePipelineManager;
            class ResourceDirectoryNode;

            class ResourceItem
            {
            public:
                typedef std::shared_ptr<ResourceItem> Handle;
                typedef std::vector<Handle> List;

                ResourceItem(ResourcePipelineManager *manager, const GUID &guid);
                ~ResourceItem(void);

                bool IsDirectoryResource(void) const;

                Handle GetParent(void) const;
                bool IsGenerated(void) const;

                const GUID &GetGUID(void) const;

                const meta::Type &GetDataType(void) const;

                const fs::path &GetSourceFileName(void) const;
                const fs::path &GetBuildFileName(void) const;
                const fs::path &GetBuildCacheFileName(void) const;

                bool HasPreview(void) const;
                const fs::path &GetPreviewFileName(void) const;

                std::string GetDisplayName(void) const;
                std::string GetRelativePathDisplayName(void) const;

                const ResourceBuildCache &GetBuildCache(void) const;

            private:
                friend class ResourcePipelineManager;

                bool m_isDirectoryResource;

                ResourcePipelineManager *m_manager;
                ResourceDirectoryNode *m_directoryNode;

                fs::path m_fileName;
                fs::path m_metaFileName;
                fs::path m_buildFileName;
                fs::path m_buildPreviewFileName;
                fs::path m_buildCacheFileName;

                GUID m_guid;

                ResourceMetaData m_metaData;
                ResourceBuildCache m_buildCache;
            };
        }
    }

    extern template 
    Json JsonSerializer::Serialize(const rp::ResourceItem::Handle &data);
}

#include "ResourceItem.hpp"