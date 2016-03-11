#pragma once
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageTextSystem.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <EntitySystem.h>
#include <Color.h>

//////////////////////
////  Predefines  ////
//////////////////////
namespace ursine
{
    namespace ecs
    {
        class SpriteText;
        class Transform;
    } // ecs namespace
} // ursine namespace

namespace game
{
    struct DamageEventArgs;
}

class DamageText;
//////////////////////
//////////////////////

class DamageTextSystem
    : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM ;

public:

    DamageTextSystem(ursine::ecs::World* world);
    ~DamageTextSystem(void);

protected:
    void OnInitialize(void) override;
    void onUpdate(EVENT_HANDLER(World));

private:
    void onDamageText(EVENT_HANDLER(game::DAMGE_TEXT_EVENT));

    void updateDamageText(const ursine::ecs::EntityHandle &damageText, DamageText* damageTextComp, float dt);

    void createDamageText(const game::DamageEventArgs* args);
    void deleteDamageText(const ursine::ecs::EntityHandle &textObject);

    ursine::resources::ResourceReference m_critText;
    ursine::resources::ResourceReference m_normText;

    std::unordered_map<ursine::ecs::EntityHandle, DamageText*> m_damageTexts;
    std::unordered_map<ursine::ecs::EntityHandle, ursine::ecs::SpriteText*> m_spriteTexts;
    std::unordered_map<ursine::ecs::EntityHandle, ursine::ecs::Transform*> m_transforms;
    
} Meta(Enable);
