#pragma once

#include "GUID.h"
#include "ResourceMetaData.h"

namespace ursine
{
    class ResourceManager;
    class ResourceData;

    class Resource
    {
    public:
        std::string GetName(void) const;

        fs::path GetPath(void) const;
        fs::path GetAbsolutePath(void) const;
        fs::path GetOutputPath(void) const;

        const ResourceMetaData &GetMeta(void) const;

    private:
        friend class ResourceManager;

        ResourceManager *m_manager;
        ResourceData *m_data;
        ResourceMetaData m_meta;
        fs::path m_path;
    };
}