#pragma once

#include "GUID.h"

namespace ursine
{
    class ResourceManager;
    class ResourceImportOptions;
    class ResourceData;

    class Resource
    {
    public:
        std::string GetName(void) const;

        fs::path GetPath(void) const;
        fs::path GetAbsolutePath(void) const;
        fs::path GetOutputPath(void) const;

        ResourceImportOptions *GetImportOptions(void);

    private:
        friend class ResourceManager;

        GUID m_guid;
        ResourceManager *m_manager;
        ResourceImportOptions *m_importOptions;
        ResourceData *m_data;
        fs::path m_path;
    };
}