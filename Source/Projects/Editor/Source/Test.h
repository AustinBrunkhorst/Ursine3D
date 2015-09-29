#pragma once

#include "Meta.h"

#include "NativeJavaScriptClass.h"

class Test : public ursine::NativeJavaScriptClass
{
    int i = 0;

public:
    Test(void) = default;

    JavaScriptMethod(PrintSomething);
    JavaScriptMethod(DoubleSomething);
    JavaScriptMethod(SquareSomething);
    JavaScriptMethod(MakeFullscreen);
} Meta(Enable);