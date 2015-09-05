#include "UrsinePrecompiled.h"

namespace ursine
{
    namespace meta
    {
        Type MetaProperty::GetType(void) const
        {
            UError( "You must use declare META_OBJECT in the class declaration." );

            return Type::Invalid;
        }
    }
}