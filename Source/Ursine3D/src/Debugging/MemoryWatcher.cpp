/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MemoryWatcher.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "MemoryWatcher.h"

#include <stdio.h>

namespace ursine
{
    void MemoryWatcher::Add(void *pointer, size_t size, const char *file, unsigned line)
    {
        MemoryAllocation allocation {
            pointer,
            size,
            line
        };

        strncpy( allocation.file, file, MAX_FILE_LENGTH );

        auto &mutex = getMutex( );

        if (mutex.try_lock( ))
        {
            getContainer( ).push_back( allocation );

            mutex.unlock( );
        }
    }

    void MemoryWatcher::Remove(void *pointer)
    {
        auto &container = getContainer( );

        if (container.empty( ))
            return;

        auto &mutex = getMutex( );

        if (mutex.try_lock( ))
        {
            for (size_t i = container.size( ) - 1u; i >= 0; --i)
            {
                if (container[ i ].pointer == pointer)
                {
                    container.erase( container.begin( ) + i );

                    mutex.unlock( );

                    return;
                }
            }

            mutex.unlock( );
        }
    }

    URSINE_TODO("Actually implement this")
    void MemoryWatcher::PrintLeaks(void)
    {
        for (auto &leak : getContainer( ))
        {
            printf( "leak: %s(%u)\n", leak.file, leak.line );
        }
    }

    MemoryWatcher::Container &MemoryWatcher::getContainer(void)
    {
        static Container container;

        return container;
    }

    std::mutex &MemoryWatcher::getMutex(void)
    {
        static std::mutex mutex;

        return mutex;
    }
}
