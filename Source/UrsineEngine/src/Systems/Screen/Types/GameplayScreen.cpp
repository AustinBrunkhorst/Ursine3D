/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GameplayScreen.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GameplayScreen.h"

#include "TimerManager.h"
#include "TweenManager.h"

namespace ursine
{
    GameplayScreen::GameplayScreen(const std::string &space_name)
        : Screen(false, space_name)
    {
        world.SetOwner(this);
    }

    void GameplayScreen::OnFocusChanged(bool state)
    {
        ecs::ScreenFocusArgs args(state);

        if (state)
        {
            gTimerManager->Resume(kTimerGroupGameplay);
            gTweenManager->Resume(kTweenGroupGameplay);
        }
        else
        {
            gTimerManager->Pause(kTimerGroupGameplay);
            gTweenManager->Pause(kTweenGroupGameplay);
        }

        world.Manager<ecs::SystemManager>()->
            Dispatch(ecs::SM_SCREEN_FOCUS_CHANGED, &args);
    }

    void GameplayScreen::Update(void)
    {
        world.Update();
    }

    void GameplayScreen::Draw(void)
    {
        world.Draw();
    }
}