#include "Test.h"

#include <Meta.h>

#include <iostream>

int main(void)
{
    auto types = ursine::meta::Type::GetTypes();

    for (auto type : types)
        std::cout << type.GetName( ) << std::endl;

    auto jason = typeof( Jason );

    auto field = jason.GetField( "myField" );

    auto range = field.GetMeta( ).GetProperty( typeof( Range ) );

    auto test = range.GetValue<Range>( );

    auto iType = typeof( int );

    auto i = iType.Create( 56 );

    auto test2 = *reinterpret_cast<int*>( i.GetValue<double>( ) );

    return 0;
}