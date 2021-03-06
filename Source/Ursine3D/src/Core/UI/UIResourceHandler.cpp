/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UIResourceHandler.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UIResourceHandler.h"

#include "BuiltInResourceConfig.h"

#include <cef_runnable.h>
#include <cef_parser.h>

namespace
{
    const auto kStatusOK = 200;
    const auto kStatusNotFound = 404;
    const auto kURLQueryDelimiter = '?';
}

namespace ursine
{
    UIResourceHandler::UIResourceHandler(const std::string &domain, resources::ResourceManager *resources)
        : m_requestPrefix( "http://"+ domain + "/" )
        , m_manager( resources )
        , m_readOffset( 0 )
        , m_resourceEntry( nullptr ) { }

    bool UIResourceHandler::ProcessRequest(
        CefRefPtr<CefRequest> request,
        CefRefPtr<CefCallback> callback
    )
    {
        auto url = request->GetURL( ).ToString( );

        // require the URL to start with the prefix
        if (url.find( m_requestPrefix ) != 0)
            return false;

        auto queryDelimiter = url.find_last_of( kURLQueryDelimiter );

        // remove queries if applicable
        if (queryDelimiter != std::string::npos)
            url = url.substr( 0, queryDelimiter );

        fs::path resourcePath = url.substr( m_requestPrefix.size( ) );

        auto extension = resourcePath.extension( ).string( );

        // remove the dot if it exists
        if (!extension.empty( ) && extension[ 0 ] == '.')
            extension.erase( extension.begin( ) );

        utils::MakeLowerCase( extension );

        // makes sure our ui layouts are treated as HTML
        if (extension == rp::kResourceTypeUIScreenExtension)
            m_mimeType = "text/html";
        else
            m_mimeType = CefGetMimeType( extension );

        auto it = resourcePath.begin( );

        // require GUID
        if (it == resourcePath.end( ))
            return false;

        GUID guid;

        try
        {
            guid = GUIDStringGenerator( )( it->string( ) );
        }
        catch (...)
        {
            UWarning( 
                "Failed to parse GUID from UI Resource path '%s'.", 
                it->string( ).c_str( ) 
            );

            // invalid GUID
            return false;
        }

        // skip over GUID
        ++it;

        fs::path subResourcePath;

        // copy the rest of the path
        for (; it != resourcePath.end( ); ++it)
            subResourcePath /= *it;

        CefPostTask( 
            TID_IO, 
            NewCefRunnableMethod( 
                this, 
                &UIResourceHandler::loadResource, 
                guid,
                subResourcePath,
                callback 
            ) 
        );

        return true;
    }

    void UIResourceHandler::GetResponseHeaders(
        CefRefPtr<CefResponse> response, 
        int64 &responseLength, 
        CefString &redirectUrl
    )
    {
        response->SetMimeType( m_mimeType );

        CefResponse::HeaderMap headers;

        headers.emplace( "Access-Control-Allow-Origin", "*" );

        
    #if defined(URSINE_WITH_EDITOR)

        // disable cache in editor mode
        headers.emplace( "Cache-Control", "no-cache, no-store" );

    #else

        // always cache for as long as possible
        headers.emplace( "Cache-Control", "max-age=290304000, public" );

    #endif

        response->SetHeaderMap( headers );

        if (m_resourceEntry)
        {
            response->SetStatus( kStatusOK );
            responseLength = m_resourceEntry->GetSize( );
        }
        else
        {
            response->SetStatus( kStatusNotFound );
            responseLength = 0;
        }
    }

    bool UIResourceHandler::ReadResponse(
        void *dataOut, 
        int bytesToRead, 
        int &bytesRead, 
        CefRefPtr<CefCallback> callback
    )
    {
        auto hasData = false;

        bytesRead = 0;

        auto entrySize = m_resourceEntry->GetSize( );

        if (m_readOffset < entrySize) 
        {
            auto transferSize = std::min( bytesToRead, static_cast<int>( entrySize - m_readOffset ));

            auto *bytes = reinterpret_cast<const byte *>( m_resourceEntry->GetData( ) );

            memcpy( dataOut, bytes + m_readOffset, transferSize );

            m_readOffset += transferSize;

            bytesRead = transferSize;

            hasData = true;
        }

        return hasData;
    }

    void UIResourceHandler::Cancel(void)
    {
        
    }

    void UIResourceHandler::loadResource(
        const GUID &guid,
        const fs::path &path,
        CefRefPtr<CefCallback> callback
    )
    {
        auto data = m_manager->LoadReference( guid );

        // doesn't exist, or isn't the correct type
        if (!data || data->GetType( ) != typeof( resources::UIProjectData ))
        {
            callback->Cancel( );

            return;
        }

        auto *resource = static_cast<resources::UIProjectData*>( data.get( ) );

        if (!resource)
        {
            callback->Cancel( );

            return;
        }

        m_resourceEntry = resource->GetEntry( path );

        if (m_resourceEntry)
            callback->Continue( );
        else
            callback->Cancel( );
    }
}
