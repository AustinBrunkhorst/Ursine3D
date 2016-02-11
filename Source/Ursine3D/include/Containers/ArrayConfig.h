/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ArrayConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

// by default, enable modification notifcations with editor
#if defined(URSINE_WITH_EDITOR) && !defined(URSINE_ARRAY_NOTIFY_MODIFICATION)

#define URSINE_ARRAY_NOTIFY_MODIFICATION

#endif

#if defined(URSINE_ARRAY_NOTIFY_MODIFICATION)

#include "EventDispatcher.h"

namespace ursine
{
    enum ArrayModifyAction
    {
        AMODIFY_INSERT,
        AMODIFY_SET,
        AMODIFY_REMOVE
    };

    typedef EventDispatcher<ArrayModifyAction, LambdaHandler> ArrayEventDispatcher;
}

#endif