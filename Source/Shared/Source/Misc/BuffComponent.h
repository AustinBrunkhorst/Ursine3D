#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BuffComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>

enum BuffType
{
    EMPOWER,
    DISEMPOWER,

    TRIGGER_DAMAGE,

    Buff_Health,
    Debuff_Health,

    Buff_End
} Meta(Enable);

class BuffComponent : public ursine::ecs::Component
{
    NATIVE_COMPONENT
public:
    static const float BUFF_INFINITE;

    BuffComponent(void);
    ~BuffComponent(void);

    void UpdateBuffTimes(const float dt);

    void AddBuff(BuffType type, const float modifier = 1.0f, const float lifeTime = BUFF_INFINITE);

    void RemoveBuff(BuffType type);
    void RemoveAllBuffs(void);
    void RemoveAllInfiniteBuffs(void);
    void RemoveAllTimedBuffs(void);

    void AddBuffTime(BuffType type, const float lifeTime = 1.0f);

protected:
    void OnInitialize(void) override;

private:
    static const int BUFF_COUNT = BuffType::Buff_End;

    // array of buff states (active == true, unactive == false)
    bool m_buffStatus[ BUFF_COUNT ];
    
    // array of modifiers for buffs
    float m_buffModifiers[ BUFF_COUNT ];

    // array of life times for buffs (-1 means infinite)
    float m_buffLifeTimes[ BUFF_COUNT ];

    void remove(int i);

} Meta(Enable, DisplayName("BuffComponent"));
