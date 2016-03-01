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

    void updateDamageText(ursine::ecs::Entity* damageText, DamageText* damageTextComp, float dt);

    void createDamageText(const game::DamageEventArgs* args);
    void deleteDamageText(ursine::ecs::Entity* textObject);

    std::string m_critText;
    std::string m_normText;

    std::unordered_map< ursine::ecs::Entity*, DamageText* > m_damageTexts;
    std::unordered_map< ursine::ecs::Entity*, ursine::ecs::SpriteText* > m_spriteTexts;
    std::unordered_map< ursine::ecs::Entity*, ursine::ecs::Transform* > m_transforms;
    
} Meta(Enable);



