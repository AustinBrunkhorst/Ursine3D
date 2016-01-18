#include "UrsinePrecompiled.h"

#include "Notification.h"
#include "NotificationManager.h"

namespace ursine
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
