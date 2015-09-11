#include "Precompiled.h"

#include <Meta.h>

#include <Application.h>

#include <iostream>

int main(int argc, char *argv[])
{
    auto types = ursine::meta::Type::GetTypes( );

    for (auto type : types)
        std::cout << type.GetName( ) << std::endl;

    ursine::Application app( argc, argv );

    app.Run( );

    return 0;
}