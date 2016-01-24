/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NativeToolHandler.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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