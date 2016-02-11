/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TwoAxisActionCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <Vec2.h>
#include "ActionCommand.h"
#include "PlayerAction.h"

template<typename T>
class TwoAxisActionCommand
    : public ActionCommandBase
{
public:
    TwoAxisActionCommand(PlayerTwoAxisAction action);

    bool Acting(void) override;

    std::shared_ptr<Command> CreateCommand(void) override;

    void SetAxis(const ursine::Vec2 &axis);

private:
    ursine::Vec2 m_axis;
    PlayerTwoAxisAction m_action;
};

template<typename T>
TwoAxisActionCommand<T>::TwoAxisActionCommand(PlayerTwoAxisAction action)
    : m_axis(ursine::Vec2(0, 0))
    , m_action(action)
{
}

template<typename T>
bool TwoAxisActionCommand<T>::Acting(void)
{
    return m_action.Acting();
}

template<typename T>
std::shared_ptr<Command> TwoAxisActionCommand<T>::CreateCommand(void)
{
    return std::shared_ptr<Command>(new T(m_action.GetAxis()));
}

template<typename T>
void TwoAxisActionCommand<T>::SetAxis(const ursine::Vec2 & axis)
{
    m_axis = axis;
}
