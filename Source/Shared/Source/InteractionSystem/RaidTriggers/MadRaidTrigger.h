#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MadRaidTriggerComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include "BaseRaidTrigger.h"
#include "Color.h"


#define NOT_MAD m_madTimer <= 0.0f
#define MAD m_madTimer > 0.0f


#define MAD_RAID_TRIGGER_FIELDS( )  \
    BASE_FIELDS( );                 \
                                    \
    EditorField(                    \
        ursine::Color MadColor,     \
        GetMadColor,                \
        SetMadColor                 \
        );                          \
                                    \
    EditorField(                    \
        float MadTime,              \
        GetMadTime,                 \
        SetMadTime                  \
        );                          \
                                    \
    EditorField(                    \
        float DealDamageTime,       \
        GetDamageTime,              \
        SetDamageTime               \
        );                          \
                                    \
    EditorField(                    \
        float MadDamage,            \
        GetDamage,                  \
        SetDamage                   \
        );                          \
                                    \
    EditorField(                    \
        float modifier,             \
        GetModifier,                \
        SetModifier                 \
        );                          \
                                    \
    EditorField(                    \
        std::string MadSymbol,      \
        GetMadSymbol,               \
        SetMadSymbol                \
        );       



class CommandQueue;

namespace ursine
{
    namespace ecs
    {
        class Entity;
        class World;
    } // ecs namespace
} // Entity namespace

class MadRaidTrigger : public BaseRaidTrigger
{
public:
    MadRaidTrigger(void);
    ~MadRaidTrigger(void);

    const ursine::Color& GetMadColor(void) const;
    void SetMadColor(const ursine::Color& color);

    const float GetMadTime(void) const;
    void SetMadTime(const float time);

    const float GetDamageTime(void) const;
    void SetDamageTime(const float time);

    const float GetDamage(void) const;
    void SetDamage(const float damage);

    const float GetModifier(void) const;
    void SetModifier(const float modifier);

    const std::string& GetMadSymbol(void) const;
    void SetMadSymbol(const std::string& symbol);

protected:
    // interaction logic
    void StartInteraction(const CommandQueue* queue, ursine::ecs::EntityHandle &entity) override;
    void Interact(const CommandQueue* queue, ursine::ecs::EntityHandle &entity) override;
    void StopInteraction(const CommandQueue* queue, ursine::ecs::EntityHandle &entity) override;

    virtual void ChildStartInteractionLogic(void) = 0;
    virtual void ChildStopInteractionLogic(void) = 0;
    virtual void ChildSuccessLogic(int id) = 0;
    virtual void ChildUpdate(void) = 0;
    
    void ChangeToMadState(int id);
    void ChangeToInteractState(void);
    void ChangeToSuccessState(void);

    void AddPlayer(const ursine::ecs::EntityHandle &player, int id);
    void ChangeEffect(int id);

    void DealDamage(const ursine::ecs::EntityHandle &player);
    void UpdateDamageTimes(float dt);

    void onUpdate(EVENT_HANDLER(WORLD_UPDATE));
    

    // color to change when mad
    ursine::Color m_madColor;

    // how long does it take trigger to reset if
    //   players make trigger mad
    float m_madTime;

    // when to deal mad damage
    float m_dealDamageTime;

    // damage to deal when mad
    float m_damage;

    // modifier for raid effect
    float m_modifier;

    // time into reset / mad timer
    float m_madTimer;

    // type
    enum TriggerType{ DISEMPOWER, EMPOWER } Meta(Enable);
    TriggerType m_triggerType;

    // UI symbol to give player
    std::string m_madSymbol;


} Meta(Disable);

