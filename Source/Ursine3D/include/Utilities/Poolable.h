/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Poolable.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

/*******************************************
* File: Poolable.h
* Author: Jordan Ellis
* Date: 11/05/2014
*
* Description:
*   Required base class for any object that 
* wishes to be poolable.
*
* How to use:
*   Implement Reset functions that make sense
* for your class.
*
* Copyright: All content © 2013-2014 DigiPen
* (USA) Corporation, all rights reserved.
********************************************/

#pragma once

#include "UrsineTypes.h"

namespace ursine
{
    class Poolable
    {
    public:

        template<typename... Args>
        void Reset(Args&&...);

        uint RefCount(void) { return _ref_count; }
        void AddRef(void) { ++_ref_count; }
        void RemoveRef(void) { --_ref_count; }

    protected:

        Poolable(void) { }

    private:

        // Count of how many handles have this object's
        // index in the pool stored.
        uint _ref_count;
    };
}