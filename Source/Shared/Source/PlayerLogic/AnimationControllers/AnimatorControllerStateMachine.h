/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimatorControllerStateMachine.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <StateMachine.h>

namespace ursine
{
    namespace ecs
    {
        class Animator;
    }
}

class AnimatorControllerStateMachine : public ursine::sm::StateMachine
{
public:
    typedef std::shared_ptr<AnimatorControllerStateMachine> Handle;

    AnimatorControllerStateMachine(const std::string &name, ursine::ecs::Animator *animator);

    ursine::ecs::Animator *GetAnimator(void);

private:
    ursine::ecs::Animator *m_animator;

    std::string m_name;
};
