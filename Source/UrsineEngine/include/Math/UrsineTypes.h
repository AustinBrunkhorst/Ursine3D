/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2014
**
** UrsineTypes.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <stdint.h>

namespace Ursine
{
    typedef uint64_t uint64;
    typedef uint32_t uint32;
    typedef uint16_t uint16;
    typedef uint8_t uint8;
    typedef uint32 uint;

    typedef int64_t int64;
    typedef int32_t int32;
    typedef int16_t int16;
    typedef int8_t int8;

    typedef uint8 byte;

    // Number of bits in a byte
    const int kBitsPerByte = 8;
}