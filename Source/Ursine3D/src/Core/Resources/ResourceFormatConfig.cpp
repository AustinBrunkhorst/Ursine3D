#include "UrsinePrecompiled.h"

#include "ResourceFormatConfig.h"

namespace ursine
{
    namespace resources
    {
        const char kResourceFormatMagic[3] { 'U', 'R', 'B' };
        const char * const kResourceFormatExtension = "urb";
        const size_t kResourceFormatHeaderSize = 32;
    }
}