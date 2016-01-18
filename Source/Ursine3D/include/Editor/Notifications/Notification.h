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
