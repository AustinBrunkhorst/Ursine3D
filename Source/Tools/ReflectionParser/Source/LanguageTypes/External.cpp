#include "Precompiled.h"

#include "External.h"

External::External(const Cursor &cursor)
    : Class( cursor, { } )
{
    
}

bool External::ShouldCompile(void) const
{
    return true;
}