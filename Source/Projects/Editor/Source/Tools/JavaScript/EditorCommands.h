/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorCommands.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
    JSFunction(CreatePlane);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreateCamera);

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
    JSFunction(CreateParticleSystem);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(CreateSpriteText);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(InspectEditorUI);

    Meta(Enable, ExposeJavaScript, EditorCommand)
    JSFunction(ReloadEditorUI);
}
