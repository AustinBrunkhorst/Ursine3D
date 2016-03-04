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
            return { guid };
        }

        ResourceData::Handle ResourceManager::LoadReference(const ResourceReference &reference, bool ignoreCache)
        {
            // if cache was ignored
            if (ignoreCache)
                return hardLoadResource( reference.GetGUID( ) );

            auto cache = m_database.find( reference.GetGUID( ) );

            // doesn't exist, attempt to hard load it
            if (cache == m_database.end( ))
                return hardLoadResource( reference.GetGUID( ) );

            return cache->second;
        }

        void ResourceManager::ReloadIfCached(const GUID &guid)
        {
            std::lock_guard<std::mutex> lock( m_databaseMutex );

            if (m_database.find( guid ) != m_database.end( ))
                hardLoadResource( guid );
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

            try
            {
                return reader.Read( fileName );
            }
            catch (AssertionException &e)
            {
                UWarning( "Failed to load resource '%s'.\nerror: %s\nfunction: %s\nfile: %s\nline: %i",
                    to_string( guid ).c_str( ),
                    e.GetErrorMessage( ).c_str( ),
                    e.GetFunction( ).c_str( ),
                    e.GetFile( ).c_str( ),
                    e.GetLine( )
                );

                return nullptr;
            }
        }

        fs::path ResourceManager::getResourceFileName(const GUID &guid) const
        {
            auto fileName = m_resourceDirectory / to_string( guid );

            fileName.replace_extension( kResourceFormatExtension );

            return fileName;
        }
    }
}