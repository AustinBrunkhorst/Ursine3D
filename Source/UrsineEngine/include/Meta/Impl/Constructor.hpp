#include "Variant.h"

namespace ursine
{
    template<typename ...Args>
    Variant Constructor::Invoke(Args &&...args) const
    {
        ArgumentList arguments { std::forward<Args>( args )... };

        return InvokeVariadic( arguments );
    }
}