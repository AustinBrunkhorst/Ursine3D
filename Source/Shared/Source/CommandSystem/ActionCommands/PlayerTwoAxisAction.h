/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerTwoAxisAction.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "PlayerAction.h"
#include <Entity.h>


class Command;

class PlayerTwoAxisAction
{
public:
    enum ActionMode
    {
        Xbox,
        Keyboard,

        ActionCount
    };
    
public:
    PlayerTwoAxisAction(void);
    PlayerTwoAxisAction(const PlayerAction &leftAction, const PlayerAction &rightAction, const PlayerAction &upAction, const PlayerAction &downAction);

    bool Acting(void);

    ursine::Vec2 GetAxis(void);

private:

    PlayerAction    m_left, 
                    m_right, 
                    m_up, 
                    m_down;
};
