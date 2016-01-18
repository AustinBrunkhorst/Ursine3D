#include "Precompiled.h"

#include "Notifications/Notification.h"
#include "Notifications/NotificationManager.h"

namespace notification
{
    Notification::Notification(NotificationID id, NotificationManager *manager)
        : m_id( id )
        , m_manager( manager )
    {
        
    }

    void Notification::Close(void)
    {
        m_manager->requestClose( m_id );
    }
}
