#include "Precompiled.h"

#include "ExplosionComponent.h"
#include "PlayerIdComponent.h"
#include "AI/AIHordeReasoner.h"
#include "HealthComponent.h"


namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(ExplosionComponent);

        static const int g_players = 0;
        static const int g_enemies = 1;

        ExplosionComponent::ExplosionComponent(): BaseComponent()
            , m_effectName("FX_ExplosionBall.uatype")
            , m_radius(2.0f)
            , m_damage(1.0f)
            , m_damageFalloffAmount(0.5)
            , m_damagePlayers(true)
            , m_damageEnemies(false)
        {
        }

        void ExplosionComponent::Explode()
        {
            Vec3 ourPosition = GetOwner()->GetTransform()->GetWorldPosition();

            EntityVector entities[2] = {};
            // if we hurt players, get anything with a player id in our radius
            if (m_damagePlayers)
            {
                entities[g_players] = GetOwner()->GetWorld()->GetEntitiesFromFilter(Filter().All< PlayerID >());
            }

            // do the same check for enemies
            if (m_damageEnemies)
            {
                entities[g_enemies] = GetOwner()->GetWorld()->GetEntitiesFromFilter(Filter().All< AIHorde >());
            }

            for (EntityVector &entVec : entities)
            {
                for (auto dmgEntity : entVec)
                {
                    float dist = dmgEntity->GetTransform()->GetWorldPosition().Distance( ourPosition );

                    if (dist < m_radius)
                    {
                        // scale the damage based off of our distance from the explosion
                        float scaledDmg = m_damage - m_damage * (dist / m_radius) * m_damageFalloffAmount;

                        // for now we are going to ignore falloff for testing
                        auto health = dmgEntity->GetComponent<Health>();

                        if (health)
                            health->DealDamage(scaledDmg);
                    } 
                }
            }

            auto explosion = GetOwner()->GetWorld()->CreateEntityFromArchetype(
                WORLD_ARCHETYPE_PATH "FX/"+ m_effectName,
                m_effectName + "Explosion"
                );



            explosion->GetTransform()->SetWorldPosition( ourPosition );
            
            GetOwner()->Delete();
        }

        const std::string& ExplosionComponent::GetEffectName() const
        {
            return m_effectName;
        }

        void ExplosionComponent::SetEffectName(const std::string& name)
        {
            m_effectName = name;
        }

        float ExplosionComponent::GetRadius() const
        {
            return m_radius;
        }

        void ExplosionComponent::SetRadius(float rad)
        {
            m_radius = rad;
        }

        float ExplosionComponent::GetDamage() const
        {
            return m_damage;
        }

        void ExplosionComponent::SetDamage(float dmg)
        {
            m_damage = dmg;
        }

        float ExplosionComponent::GetDamageFallOff() const
        {
            return m_damageFalloffAmount;
        }

        void ExplosionComponent::SetDamageFallOff(float falloff)
        {
            m_damageFalloffAmount = falloff;
        }

        bool ExplosionComponent::GetDamagePlayers() const
        {
            return m_damagePlayers;
        }

        void ExplosionComponent::SetDamagePlayers(bool dmg)
        {
            m_damagePlayers = dmg;
        }

        bool ExplosionComponent::GetDamageEnemies() const
        {
            return m_damagePlayers;
        }

        void ExplosionComponent::SetDamageEnemies(bool dmg)
        {
            m_damagePlayers = dmg;
        }
    }
}
