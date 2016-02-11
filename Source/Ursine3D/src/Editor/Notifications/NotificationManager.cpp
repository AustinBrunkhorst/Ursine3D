/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NotificationManager.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "NotificationManager.h"

#include "UIView.h"

namespace
{
    namespace channel
    {
        const auto EditorNotification = "EditorNotification";
    }

    namespace events
    {
        const auto NotificationCreated = "created";
        const auto NotificationClose = "close";
    }
}

namespace ursine
{
    struct NotificationManager::NotificationInternal
    {
        NotificationConfig config;

        NotificationInternal(const NotificationConfig &config)
            : config( config ) { }
    };

    NotificationManager::NotificationManager(void)
        : m_nextID( 0 )
        , m_ui( nullptr )
    {
        
    }

    void NotificationManager::SetUI(const UIView::Handle &ui)
    {
        m_ui = ui;
    }

    Notification NotificationManager::Create(const NotificationConfig &config)
    {
        std::lock_guard<std::mutex> lock( m_mutex );

        auto id = m_nextID++;

        auto notification = std::make_shared<NotificationInternal>( config );

        m_notifications[ id ] = notification;

        Json::array buttons;

        for (auto &button : config.buttons)
        {
            buttons.emplace_back( Json::object {
                { "text", button.text }
            } );
        }

        auto eventData = Json::object {
            { "id", static_cast<int>( id ) },
            { "type", static_cast<int>( config.type ) },
            { "dismissible", config.dismissible },
            { "header", config.header },
            { "message", config.message },
            { "duration", config.duration.Milliseconds( ) },
            { "buttons", buttons }
        };

        UAssert( m_ui != nullptr, 
            "Invalid UI handle for NotificationManager" 
        );

        m_ui->Message(
            UI_CMD_BROADCAST,
            channel::EditorNotification,
            events::NotificationCreated, 
            eventData
        );

        return { id, this };
    }

    void NotificationManager::requestClose(NotificationID id)
    {
        auto eventData = Json::object{
            { "id", static_cast<int>( id ) },
        };

        UAssert( m_ui != nullptr,
            "Invalid UI handle for NotificationManager"
        );

        m_ui->Message(
            UI_CMD_BROADCAST,
            channel::EditorNotification,
            events::NotificationClose,
            eventData
        );
    }

    void NotificationManager::onButtonClicked(NotificationID id, NotificationButtonID buttonID)
    {
        std::lock_guard<std::mutex> lock( m_mutex );

        auto search = m_notifications.find( id );

        UAssert( search != m_notifications.end( ), 
            "Invalid notification on button event.\nID: %u",
            id
        );

        auto &buttons = search->second->config.buttons;

        UAssert( buttonID < buttons.size( ),
            "Invalid notification button ID.\nID: %u, Button ID: %u",
            id,
            buttonID
        );

        auto &button = buttons[ buttonID ];

        Notification notification( id, this );

        if (button.onClick)
            button.onClick( notification );
    }

    void NotificationManager::onAfterClose(NotificationID id)
    {
        std::lock_guard<std::mutex> lock( m_mutex );

        auto search = m_notifications.find( id );

        UAssert( search != m_notifications.end( ), 
            "Invalid notification closed." 
        );

        auto &callback = search->second->config.closeCallback;

        Notification notification( id, this );

        if (callback)
            callback( notification );

        m_notifications.erase( search );
    }
}