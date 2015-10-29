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

    void onViewportInvalidated(EVENT_HANDLER( NativeEditorTool ));

} Meta(Enable);