/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Main.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Test.h"

#include <Meta.h>

#include <iostream>

#include "MetaTestReflectionModule.h"
#include <UrsineReflectionModule.h>

#include <UrsineLogTools.h>
using namespace ursine;
using namespace meta;

class Test
{
public:
    Test(int foo)
        : m_foo( foo ) { }

    void Foo()
    {
        
    }

private:
    int m_foo;
};

int main(void)
{
    MethodInvoker<Test, void> invoker( &Test::Foo );

    Variant instance = Test( 5 );

    auto result = invoker.Invoke( instance, { } );

    return 0;
}