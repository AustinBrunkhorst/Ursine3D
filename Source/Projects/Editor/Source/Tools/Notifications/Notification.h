#pragma once

#include "Notifications/NotificationConfig.h"

namespace notification 
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
