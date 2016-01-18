/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DebugUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "NotificationUtils.h"

#include "Editor.h"

namespace ursine
{
    Notification EditorPostNotification(const NotificationConfig &config)
    {
        auto *editor = GetCoreSystem( Editor );

        return editor->PostNotification( config );
    }

    JSFunction(NotificationButtonCallback)
    {
        auto *editor = GetCoreSystem( Editor );

        auto id = arguments[ 0 ]->GetUIntValue( );
        auto buttonID = arguments[ 1 ]->GetUIntValue( );

        editor->GetNotificationManager( ).onButtonClicked( id, buttonID );

        return CefV8Value::CreateBool( true );
    }

    JSFunction(NotificationCloseCallback)
    {
        auto *editor = GetCoreSystem( Editor );

        auto id = arguments[ 0 ]->GetUIntValue( );

        editor->GetNotificationManager( ).onAfterClose( id );

        return CefV8Value::CreateBool( true );
    }
}
