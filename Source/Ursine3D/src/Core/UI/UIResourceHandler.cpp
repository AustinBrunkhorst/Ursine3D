#include "UrsinePrecompiled.h"

#include "UIResourceHandler.h"

#include <cef_runnable.h>

namespace ursine
{
    UIResourceHandler::UIResourceHandler(const std::string &scheme, resources::ResourceManager *resources)
        : m_requestPrefix( scheme +":/" )
        , m_resources( resources )
        , m_uiResource( nullptr ) { }

    bool UIResourceHandler::ProcessRequest(
        CefRefPtr<CefRequest> request,
        CefRefPtr<CefCallback> callback
    )
    {
        auto url = request->GetURL( ).ToString( );

        // require the URL to start with the prefix
        if (url.find( m_requestPrefix ) != 0)
            return false;

        fs::path resourcePath = url.substr( m_requestPrefix.size( ) );

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
        
    }

    bool UIResourceHandler::ReadResponse(
        void *dataOut, 
        int bytesToRead, 
        int &bytesRead, 
        CefRefPtr<CefCallback> callback
    )
    {
        return false;
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
        auto data = m_resources->LoadReference( guid );

        // doesn't exist, or isn't the correct type
        if (!data || data->GetType( ) != typeof( resources::UIResourceData ))
        {
            callback->Cancel( );

            return;
        }

        m_uiResource = static_cast<resources::UIResourceData*>( data.get( ) );
        m_subResourcePath = path;

        callback->Continue( );
    }
}
