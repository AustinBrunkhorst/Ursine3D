/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MemoryWatcher.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "MemoryAllocation.h"

#include <vector>
#include <mutex>

namespace ursine
{
    class MemoryWatcher
    {
        typedef std::vector<MemoryAllocation> Container;

        static Container &getContainer(void);
        static std::mutex &getMutex(void);

    public:
        static void Add(void *pointer, size_t size, const char *file, unsigned line);
        static void Remove(void *pointer);

        static void PrintLeaks(void);
    };
}
