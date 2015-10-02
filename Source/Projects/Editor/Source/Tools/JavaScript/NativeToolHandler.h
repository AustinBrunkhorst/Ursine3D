#pragma once

#include <JSConfig.h>
#include <NativeJSClass.h>

#include "NativeEditorTool.h"

class NativeToolHandler : public ursine::NativeJSClass
{
    JAVASCRIPT_CLASS;

public:
    JSConstructor(NativeToolHandler);

    virtual ~NativeToolHandler(void);

    JSMethod(Event);

private:
    NativeEditorTool *m_tool;
} Meta(Enable);