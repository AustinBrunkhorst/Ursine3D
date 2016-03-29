#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BaseRaidTrigger.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include "BaseInteraction.h"
#include <Entity.h>
#include "Color.h"
#include "BuffComponent.h"


#define BASE_FIELDS( )                \
    EditorField(                      \
        BuffType RaidEffect,          \
        GetRaidEffect,                \
        SetRaidEffect                 \
        );                            \
                                      \
    EditorField(                      \
        std::string ParticleEffect,   \
        GetEmitter,                   \
        SetEmitter                    \
        );                            \
                                      \
    EditorField(                      \
        std::string UISymbol,         \
        GetUISymbol,                  \
        SetUISymbol                   \
        );                            \
                                      \
    EditorField(                      \
        ursine::Color SuccessColor,   \
        GetSuccessColor,              \
        SetSuccessColor               \
        );                            \
                                      \
    EditorField(                      \
        ursine::Color InteractColor,  \
        GetInteractColor,             \
        SetInteractColor              \
        );                            


class CommandQueue;

namespace ursine
{
    namespace ecs
    {
        class ParticleColorAnimator;
        class Entity;
    } // ecs namespace
} // ursine namespace


class BaseRaidTrigger : public Interaction
{
public:

    BaseRaidTrigger(void);
    ~BaseRaidTrigger(void);

    const BuffType GetRaidEffect(void) const;
    void SetRaidEffect(const BuffType effect);

    const std::string GetEmitter(void) const;
    void SetEmitter(std::string archetype);

    const ursine::Color& GetSuccessColor(void) const;
    void SetSuccessColor(const ursine::Color& color);

    const ursine::Color& GetInteractColor(void) const;
    void SetInteractColor(const ursine::Color& color);

    const std::string& GetUISymbol(void);
    void SetUISymbol(const std::string& symbol);

protected:
    void Initialize(ursine::ecs::Entity* owner);

    void SetAnimatorColors(const ursine::Color& color);

    virtual void onDeath(EVENT_HANDLER(ursine::ecs::Entity));

    virtual void onPlayerDeath(EVENT_HANDLER(ENTITY_REMOVED));

    struct Player
    {
        BuffComponent* buffComp;
        BuffType effectActive;
        float activeTime;

        Player( );
        void init(BuffComponent* buffComp, BuffType effect, float time = 0.0f);
        void uninit(void);

        void changeEffect(BuffType effect);
        void decrementTime(float dt);
    };

    void RemovePlayer(int id);
    void ClearPlayers(void);

    Player m_players[ 2 ];

    // how many players are on trigger
    int m_playerCount;

    // what effect will this trigger emit
    BuffType m_raidEffect;

    // what effect will trigger currently give
    BuffType m_currEffect;

    // Particle effect to show state of trigger
    ursine::ecs::ParticleColorAnimator* m_pAnimator;

    // use color to show successful interaction
    ursine::Color m_successColor;

    // use to show that players may interact with
    ursine::Color m_interactColor;

    // UI symbol to give player
    std::string m_uiSymbol;

    // archetype to load
    std::string m_archetypeToLoad;

    bool m_active;

} Meta( Disable ) ;

