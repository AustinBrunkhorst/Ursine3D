/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIScreenConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UIScreenState.h"
#include "UrsineTypes.h"

namespace ursine
{
    class UIScreen;

    typedef uint32 UIScreenID;

    struct UIScreenConfig
    {
        bool inputBlocking;
        int priority;

        UIScreenConfig(bool inputBlocking = true, int priority = 0)
            : inputBlocking( inputBlocking )
            , priority( priority ) { }
    };

    struct UIScreenMessageArgs : EventArgs
    {
        const std::string &message;
        const Json &data;

        UIScreenMessageArgs(const std::string &message, const Json &data)
            : message( message )
            , data( data ) { }
    };
}