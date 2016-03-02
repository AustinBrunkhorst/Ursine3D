/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineAIStateMachine.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <StateMachine.h>

class VineAI;

class VineAIStateMachine : public ursine::sm::StateMachine
{
public:
    typedef std::shared_ptr<VineAIStateMachine> Handle;

    VineAIStateMachine(VineAI *ai);

    void Update(void) override;

    VineAI *GetAI(void);

private:
    VineAI *m_ai;
};
