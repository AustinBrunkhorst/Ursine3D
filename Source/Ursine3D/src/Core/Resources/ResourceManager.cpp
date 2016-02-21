#include "UrsinePrecompiled.h"

#include "ResourceManager.h"

#include "ResourceFormatConfig.h"
#include "ResourceFormatReader.h"

namespace ursine 
{
    namespace resources
    {
        ResourceManager::ResourceManager(void) { }
        ResourceManager::~ResourceManager(void) { }

        void ResourceManager::SetResourceDirectory(const fs::path &resourceDirectory)
        {
            m_resourceDirectory = resourceDirectory;
        }

        ResourceReference ResourceManager::CreateReference(const GUID &guid)
        {
            return { this, guid };
        }

        ResourceData::Handle ResourceManager::LoadReference(const ResourceReference &reference)
        {
            auto cache = m_database.find( reference.m_resourceGUID );

            // doesn't exist, attempt to hard load it
            if (cache == m_database.end( ))
                return loadResource( reference.m_resourceGUID );

            return cache->second;
        }

        ResourceData::Handle ResourceManager::loadResource(const GUID &guid)
        {
            auto fileName = getResourceFileName( guid );

            // build doesn't exist
            if (!exists( fileName ))
                return nullptr;

            ResourceFormatReader reader;

            return reader.Read( fileName );
        }

        fs::path ResourceManager::getResourceFileName(const GUID &guid)
        {
            auto fileName = m_resourceDirectory / to_string( guid );

            fileName.replace_extension( kResourceFormatExtension );

            return fileName;
        }
    }
}