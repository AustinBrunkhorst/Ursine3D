#pragma once

namespace ursine
{
    class ResourceManager;
    class Resource;
    class ResourceImportOptions;

    class ResourceOutputContext
    {
    public:
        std::fstream GetOutputFileHandle(void) const;

    private:
        friend class ResourceManager;

        Resource *m_resource;

        ResourceOutputContext(Resource *resource);
    };
}