/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Notification.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

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
