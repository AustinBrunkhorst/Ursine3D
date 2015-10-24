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

    float m_camZoom;

    ursine::SVec3 m_camPos;
    
    ursine::graphics::Camera &getEditorCamera(void);

    void onAppUpdate(EVENT_HANDLER( ursine::Application ));

    void onViewportInvalidated(EVENT_HANDLER( NativeEditorTool ));

    void onMouseScroll(EVENT_HANDLER( ursine::MouseManager ));

    void UpdateCameraKeys(float dt);
    void UpdateCameraMouse(float dt);

} Meta(Enable);