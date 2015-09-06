#include "Test.h"

#include <Meta.h>

#include <iostream>

int main(void)
{
    auto types = ursine::meta::Type::GetTypes( );

    for (auto type : types)
        std::cout << type.GetName( ) << std::endl;

    // from compile type name
    auto jasonType = typeof( Jason );

    // or from string name
    auto rangeType = ursine::meta::Type::Get( "Range" );

    auto field = jasonType.GetField( "myField" );

    auto &meta = field.GetMeta( );

    // dynamically getting the properties based on their runtime deduced type
    auto range = meta.GetProperty( rangeType );
    auto slider = meta.GetProperty( rangeType );

    // dynamic fields
    auto min = rangeType.GetField( "min" );
    auto max = rangeType.GetField( "max" );

    // compile time value
    auto rangeValue = range.GetValue<Range>( );

    // -3000
    auto dynamicMin = min.GetValue( range ).GetValue<int>( );

    // 50 
    auto dynamicMax = max.GetValue( range ).GetValue<int>( );

    return 0;
}