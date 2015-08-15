/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MemoryAllocation.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace Ursine
{
    // maximum length of a filename
    const int MAX_FILE_LENGTH = 256;

    struct MemoryAllocation
    {
        // location of the memory allocated
        void *pointer;

        // number of bytes allocated
        size_t size;

        // line number in the file allocated
        unsigned line;

        // filename memory was allocated in
        char file[256];
    };
}
