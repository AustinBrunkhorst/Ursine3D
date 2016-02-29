/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossVineStateMachine.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <StateMachine.h>

class BossVineAI;

class BossVineStateMachine : public ursine::sm::StateMachine
{
public:
    typedef std::shared_ptr<BossVineStateMachine> Handle;

    BossVineStateMachine(BossVineAI *ai);

    BossVineAI *GetAI(void);

private:
    BossVineAI *m_ai;
};
