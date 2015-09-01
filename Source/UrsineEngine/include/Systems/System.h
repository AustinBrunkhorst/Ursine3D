/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** System.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    // forward declaration
    class SystemContainer;

    class System
    {
        friend class SystemContainer;

        // determines if this system updates when its owner is in a paused state
        bool m_updateOnPause;

    protected:
        // container that owns this system
        SystemContainer *m_owner;

    public:
        explicit System(bool updateOnPause = false);

        // This must be implemented here, or there will be linker errors
        virtual ~System(void) {}

        // Called when the system is updated
        virtual void Update(void) {}

        // Called when the system is drawn
        virtual void Draw(void) {}
    };
}