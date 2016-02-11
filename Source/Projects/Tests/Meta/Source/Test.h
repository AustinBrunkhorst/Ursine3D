/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Test.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Meta.h>

class BaseClass { };

class Testing : public BaseClass
{
public:
    double potato;
    double what;

    int Foo(void) const { return 5; }
} Meta(Enable);