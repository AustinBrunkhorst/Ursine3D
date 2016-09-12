/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorSceneManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Scene.h>

class Editor;
class Project;

class EditorSceneManager
{
public:
    EditorSceneManager(Project *project);
    ~EditorSceneManager(void);

private:
    Editor *m_editor;
    Project *m_project;

    ///////////////////////////////////////////////////////////////////////////
    // Scene Events
    ///////////////////////////////////////////////////////////////////////////

    void onSceneActiveWorldChanged(EVENT_HANDLER(ursine::Scene));
    void onScenePlayStateChanged(EVENT_HANDLER(ursine::Scene));
    void onSceneFrameStepped(EVENT_HANDLER(ursine::Scene));
};