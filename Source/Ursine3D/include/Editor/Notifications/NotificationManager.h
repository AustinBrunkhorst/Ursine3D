/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NotificationManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Notification.h"
#include "UIView.h"

#include "NativeJSFunction.h"

namespace ursine
{
    class NotificationManager
    {
    public:
        NotificationManager(void);

        void SetUI(const UIView::Handle &ui);

        Notification Create(const NotificationConfig &config);

    private:
        friend class Notification;
        friend JSFunction(NotificationButtonCallback);
        friend JSFunction(NotificationCloseCallback);

        struct NotificationInternal;

        NotificationID m_nextID;

        std::mutex m_mutex;

        UIView::Handle m_ui;

        std::unordered_map<
            NotificationID, 
            std::shared_ptr<NotificationInternal>
        > m_notifications;

        void requestClose(NotificationID id);

        void onButtonClicked(NotificationID id, NotificationButtonID buttonID);
        void onAfterClose(NotificationID id);
    };
}