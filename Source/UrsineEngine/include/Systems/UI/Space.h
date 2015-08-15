/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Space.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ScreenConfig.h"
#include "SpaceConfig.h"

namespace Ursine
{
    class Screen;

    namespace UI
    {
        class Space : public EventDispatcher<std::string>
        {
            friend class Screen;

            // determines if this screen has been entered yet
            bool _entered;

            // id of the screen that owns this space
            ScreenID _owner;            

            // name of this space
            std::string _name;

            Space &operator=(const Space &rhs) = delete;
        public:
            Space(const std::string &name);

            // Called when this space is entered
            void Entered(const Json &data);

            // Called when this space is requesting to exit
            void Exited(void);

            // Gets the name of this space
            const std::string &GetName(void) const;

            // Sends a message to the space in JavaScript
            void Message(const std::string &name, const Json &data) const;
        };
    }
}