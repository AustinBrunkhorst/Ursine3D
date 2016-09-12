/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorScene.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Scene.h>

#include "ScenePlayState.h"

#include "EditorSceneEvent.h"

class EditorScene : public ursine::Scene
{
public:
    EditorScene(void);

    ScenePlayState GetPlayState(void) const;
    void SetPlayState(ScenePlayState state);

    bool IsPaused(void) const override;

    void Step(void) const;

    void Update(ursine::DeltaTime dt) const override;
    void Render(void) const override;

private:
    ScenePlayState m_playState;
};