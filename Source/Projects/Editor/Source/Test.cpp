#include "Precompiled.h"

#include "Test.h"

JavaScriptMethod(Test::PrintSomething)
{
    printf( "from javascript: %s\n", arguments[0]->GetStringValue( ).ToString( ).c_str( ) );

    return CefV8Value::CreateInt( i++ );
}

JavaScriptMethod(Test::DoubleSomething)
{
    return CefV8Value::CreateDouble( arguments[ 0 ]->GetDoubleValue( ) * 2.0 );
}

JavaScriptMethod(Test::SquareSomething)
{
    auto value = arguments[ 0 ]->GetDoubleValue( );

    return CefV8Value::CreateDouble( value * value );
}