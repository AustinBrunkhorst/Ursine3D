/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Notification.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "NotificationConfig.h"

namespace ursine 
{
    class NotificationManager;

    class Notification
    {
    public:
        void Close(void);

    private:
        friend class NotificationManager;

        Notification(NotificationID id, NotificationManager *manager);

        NotificationID m_id;
        NotificationManager *m_manager;
    };
}
