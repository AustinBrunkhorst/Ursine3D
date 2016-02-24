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
            class ResourceItem
            {
            public:
                typedef std::shared_ptr<ResourceItem> Handle;
                typedef std::vector<Handle> List;

                ResourceItem(const GUID &guid);

                const GUID &GetGUID(void) const;

                const meta::Type &GetDataType(void) const;

                const fs::path &GetSourceFileName(void) const;
                const fs::path &GetBuildFileName(void) const;

                bool HasPreview(void) const;
                const fs::path &GetPreviewFileName(void) const;

                std::string GetDisplayName(void) const;

            private:
                friend class ResourcePipelineManager;

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
}