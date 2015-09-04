#include "Test.h"

#include <Meta.h>
#include <Meta/Method.h>

#include <iostream>

int main(void)
{
    auto types = ursine::meta::Type::GetTypes( );

    for (auto type : types)
    {
        std::cout << type.GetName( ) << std::endl;
    }

    system( "pause" );

    return 0;
}