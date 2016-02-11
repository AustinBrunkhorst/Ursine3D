/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SceneView.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "NativeEditorTool.h"

#include "Editor.h"
#include "SVec3.h"

class SceneView : public NativeEditorTool
{
public:
    Meta(DisableNonDynamic)
    SceneView(JSHandlerArgs);

    virtual ~SceneView(void);

private:
    Editor *m_editor;

    void onFocusChanged(EVENT_HANDLER(NativeEditorTool));
    void onMouseFocusChanged(EVENT_HANDLER(NativeEditorTool));

    void onViewportInvalidated(EVENT_HANDLER(NativeEditorTool));

} Meta(Enable);