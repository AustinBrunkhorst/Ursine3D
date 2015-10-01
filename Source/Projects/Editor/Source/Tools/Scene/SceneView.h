#pragma once

#include "NativeEditorTool.h"

#include "Editor.h"

class SceneView : public NativeEditorTool
{
public:
    Meta(DisableNonDynamic)
    SceneView(JSHandlerArgs);

    virtual ~SceneView(void);

private:
    Editor *m_editor;

    float m_camZoom;

    GFXCamera &getEditorCamera(void);

    void onViewportInvalidated(EVENT_HANDLER(NativeEditorTool));

    void onMouseScroll(EVENT_HANDLER(ursine::MouseManager));
    void onMouseMove(EVENT_HANDLER(ursine::MouseManager));

    void onKeyDown(EVENT_HANDLER(ursine::KeyboardManager));
} Meta(Enable);