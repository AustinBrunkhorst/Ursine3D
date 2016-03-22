/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossAIStateMachine.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <StateMachine.h>

class BossAI;

class BossAIStateMachine : public ursine::sm::StateMachine
{
public:
    typedef std::shared_ptr<BossAIStateMachine> Handle;

    // Global variables for the boss
    static const std::string VineCount;

    BossAIStateMachine(BossAI *boss);

    void Update(void) override;

    BossAI *GetBoss(void);

private:
    BossAI *m_boss;
};
