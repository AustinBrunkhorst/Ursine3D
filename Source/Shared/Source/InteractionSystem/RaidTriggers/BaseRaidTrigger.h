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
#include <ArchetypeData.h>
#include "BaseInteraction.h"
#include <Entity.h>
#include "Color.h"
#include "BuffComponent.h"


#define BASE_FIELDS( )                        \
    EditorField(                              \
        BuffType RaidEffect,                  \
        GetRaidEffect,                        \
        SetRaidEffect                         \
    );                                        \
                                              \
    EditorResourceField(                      \
        ursine::resources::ArchetypeData,     \
        ParticleEffect,                       \
        GetEmitter,                           \
        SetEmitter                            \
    );                                        \
                                              \
    EditorResourceField(                      \
        ursine::resources::ResourceReference, \
        UISymbol,                             \
        GetUISymbol,                          \
        SetUISymbol                           \
    );                                        \
                                              \
    EditorField(                              \
        ursine::Color SuccessColor,           \
        GetSuccessColor,                      \
        SetSuccessColor                       \
    );                                        \
                                              \
    EditorField(                              \
        ursine::Color InteractColor,          \
        GetInteractColor,                     \
        SetInteractColor                      \
    );                                        \

class CommandQueue;

namespace ursine
{
    namespace ecs
    {
        class ParticleColorAnimator;
        class Entity;
    }
}

class BaseRaidTrigger : public Interaction
{
public:

    BaseRaidTrigger(void);
    ~BaseRaidTrigger(void);

    const BuffType GetRaidEffect(void) const;
    void SetRaidEffect(const BuffType effect);

    const ursine::resources::ResourceReference &GetEmitter(void) const;
    void SetEmitter(const ursine::resources::ResourceReference &archetype);

    ursine::ecs::ParticleColorAnimator *GetAnimator(void) const;

    const ursine::Color& GetSuccessColor(void) const;
    void SetSuccessColor(const ursine::Color& color);

    const ursine::Color& GetInteractColor(void) const;
    void SetInteractColor(const ursine::Color& color);

    const ursine::resources::ResourceReference &GetUISymbol(void);
    void SetUISymbol(const ursine::resources::ResourceReference &symbol);

protected:
    void Initialize(const ursine::ecs::EntityHandle &owner);

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
    ursine::resources::ResourceReference m_uiSymbol;

    // archetype to load
    ursine::resources::ResourceReference m_archetypeToLoad;

    bool m_active;

} Meta( Disable ) ;

