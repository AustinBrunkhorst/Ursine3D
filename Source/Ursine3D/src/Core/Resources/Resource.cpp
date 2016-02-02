#include "UrsinePrecompiled.h"

#include "Resource.h"

#include "ResourceManager.h"
#include "ResourceImportOptions.h"
#include "ResourceData.h"

namespace ursine 
{
    namespace
    {
        const auto kResourceOuputExtension = "asset";
        const auto kResourceOuputDirectory = "Resources";
    }

    std::string Resource::GetName(void) const
    {
        return change_extension( m_path.filename( ), "" ).string( );
    }

    fs::path Resource::GetPath(void) const
    {
        return m_path;
    }

    fs::path Resource::GetAbsolutePath(void) const
    {
        return m_manager->m_assetDirectory / m_path;
    }

    fs::path Resource::GetOutputPath(void) const
    {
        fs::path outputPath = 
            m_manager->m_libraryDirectory / kResourceOuputDirectory / to_string( m_guid );

        outputPath.replace_extension( kResourceOuputExtension );

        return outputPath;
    }

    const ResourceMetaData &Resource::GetMeta(void) const
    {
        return m_meta;
    }
}