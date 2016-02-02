#include "UrsinePrecompiled.h"

#include "ResourceManager.h"

#include "Resource.h"

namespace ursine 
{
    CORE_SYSTEM_DEFINITION( ResourceManager );

    void ResourceManager::SetAssetDirectory(const fs::path &assetDirectory)
    {
        m_assetDirectory = assetDirectory;
    }

    void ResourceManager::SetLibraryDirectory(const fs::path &libraryDirectory)
    {
        m_libraryDirectory = libraryDirectory;
    }
}