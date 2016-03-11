/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ExplosionComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include <Component.h>
#include <ArchetypeData.h>
namespace ursine
{
    namespace ecs
    {
        class ExplosionComponent : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorResourceField(
                ursine::resources::ArchetypeData,
                explosionEffectArchetype,
                GetEffectArchetype,
                SetEffectArchetype
            );

            EditorField(
                float Radius,
                GetRadius,
                SetRadius
            );

            EditorField(
                float Damage,
                GetDamage,
                SetDamage
            );

            EditorField(
                float DamageFalloffPercent,
                GetDamageFallOff,
                SetDamageFallOff
            );
            
            EditorField(
                bool DamagesPlayers,
                GetDamagePlayers,
                SetDamagePlayers
            );

            EditorField(
                bool DamagesEnemies,
                GetDamageEnemies,
                SetDamageEnemies
            );

            ExplosionComponent(void);

            // function that is called when you want the object to explode
            void Explode(void);

            ////////////////////////////////////////////////////////////////////
            // Getters and Setters
            ////////////////////////////////////////////////////////////////////
            const ursine::resources::ResourceReference &GetEffectArchetype(void) const;
            void SetEffectArchetype(const ursine::resources::ResourceReference &name);

            float GetRadius(void) const;
            void SetRadius(float rad);

            float GetDamage(void) const;
            void SetDamage(float dmg);

            float GetDamageFallOff(void) const;
            void SetDamageFallOff(float falloff);

            bool GetDamagePlayers(void) const;
            void SetDamagePlayers(bool dmg);

            bool GetDamageEnemies(void) const;
            void SetDamageEnemies(bool dmg);

        private:
            ursine::resources::ResourceReference m_effectArchetype;

            // radius of explosion
            float m_radius;

            // how much damage the explosion does in the center
            float m_damage;
            
            // this determines difference in damage from edge to center
            float m_damageFalloffAmount;

            // determines if the explosion damages players
            bool m_damagePlayers;

            // determines if the explosion damages enemies
            bool m_damageEnemies;

        } Meta(Enable, DisplayName( "Explosion" ));
    }
}