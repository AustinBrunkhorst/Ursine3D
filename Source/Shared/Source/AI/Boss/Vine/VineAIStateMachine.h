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
class Health;

class VineAIStateMachine : public ursine::sm::StateMachine
{
public:
    typedef std::shared_ptr<VineAIStateMachine> Handle;

    static const std::string WhipCooldown;
    static const std::string UprootCooldown;

    static const std::string InRange;
    static const std::string InView;

    static const std::string GoHome;
    static const std::string JumpToHome;
    static const std::string IsHome;

    static const std::string PursueTarget;

    static const std::string Dead;

    VineAIStateMachine(VineAI *ai);

    void Initialize(void);

    void Update(void) override;

    VineAI *GetAI(void);

private:
    VineAI *m_ai;

    void decrementCooldown(const std::string &name);

    void onDeath(EVENT_HANDLER(Health));
};
