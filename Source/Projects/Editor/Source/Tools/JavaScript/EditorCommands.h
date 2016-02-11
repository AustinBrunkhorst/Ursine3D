#pragma once

#include <NativeJSFunction.h>

/** @brief Exposes a function that is used as a command in the editor
 */
struct EditorCommand : ursine::meta::MetaProperty
{
    META_OBJECT;
};

namespace editor_commands
{
    Meta(Enable, ExposeJavaScript)
    JSFunction(GetEditorCommands);

    ///////////////////////////////////////////////////////////////////////////

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreateEmptyEntity);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreateEntityFromArchetype);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreatePlane);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreateBox);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreateCylinder);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreateSphere);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreatePointLight);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreateSpotLight);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreateDirectionalLight);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(InspectEditorUI);
}
