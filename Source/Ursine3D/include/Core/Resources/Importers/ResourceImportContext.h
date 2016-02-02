#pragma once

namespace ursine
{
    class ResourceManager;
    class Resource;
    class ResourceImportOptions;

    class ResourceImportContext
    {
    public:
        std::ifstream GetInputFileHandle(void) const;

        template<typename OptionsType>
        OptionsType *GetImportOptions(void);

    private:
        friend class ResourceManager;

        Resource *m_resource;

        ResourceImportContext(Resource *resource);
    };
}

#include "ResourceImportContext.hpp"