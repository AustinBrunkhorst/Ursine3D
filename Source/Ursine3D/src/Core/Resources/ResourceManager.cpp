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

        ResourceData::Handle ResourceManager::LoadReference(const ResourceReference &reference, bool ignoreCache)
        {
            // if cache was ignored
            if (ignoreCache)
                return hardLoadResource( reference.m_resourceGUID );

            auto cache = m_database.find( reference.m_resourceGUID );

            // doesn't exist, attempt to hard load it
            if (cache == m_database.end( ))
                return hardLoadResource( reference.m_resourceGUID );

            return cache->second;
        }

        bool ResourceManager::ResourceExists(const GUID &guid) const
        {
            return m_database.find( guid ) != m_database.end( ) || exists( getResourceFileName( guid ) );
        }

        ResourceData::Handle ResourceManager::hardLoadResource(const GUID &guid)
        {
            auto resource = loadResource( guid );

            // if it was loaded, cache it
            if (resource)
                m_database[ guid ] = resource;

            return resource;
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

        fs::path ResourceManager::getResourceFileName(const GUID &guid) const
        {
            auto fileName = m_resourceDirectory / to_string( guid );

            fileName.replace_extension( kResourceFormatExtension );

            return fileName;
        }
    }
}