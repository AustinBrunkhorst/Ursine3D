/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsineMemory.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

//#ifdef CONFIG_DEBUG
//#define TRACK_MEMORY
//#endif

#ifdef TRACK_MEMORY

#include <MemoryWatcher.h>

inline void *operator new(unsigned int size)
{
    void *pointer = malloc(size);

    UAssert(pointer, "Bad allocation.");

    return pointer;
}

inline void* operator new(unsigned int size, const char *filename, int line)
{
    void *pointer = malloc(size);

    UAssert(pointer, "Bad allocation.");

    Ursine::MemoryWatcher::Add(pointer, size, filename, line);

    return pointer;
}

inline void operator delete(void *pointer)
{
    Ursine::MemoryWatcher::Remove(pointer);

    free(pointer);
}

inline void operator delete(void *pointer, const char *filename, int line)
{
    Ursine::MemoryWatcher::Remove(pointer);

    free(pointer);
}

#define new new(__FILE__, __LINE__)

#endif // TRACK_MEMORY