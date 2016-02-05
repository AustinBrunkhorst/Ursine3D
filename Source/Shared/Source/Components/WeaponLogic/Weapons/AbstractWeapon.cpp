
#include "Precompiled.h"
#include "AbstractWeapon.h"
#include "AmmoPickupComponent.h"
#include "GameEvents.h"
#include <CollisionEventArgs.h>
#include <Entity.h>
#include <RigidbodyComponent.h>
#include <TransformComponent.h>
#include <BoxColliderComponent.h>
#include "InteractableComponent.h"
#include "WeaponPickup.h"
#include <AnimatorComponent.h>

using namespace ursine;

namespace
{
    float clamp(float min, float max, float val)
    {
        return ( min > val ) ? min : ( ( max < val ) ? max : val );
    }
}


AbstractWeapon::AbstractWeapon(void) :
    m_owner( nullptr ),
    m_damageToApply(1.0f),
    m_critModifier(1.0f),
    m_damageInterval(1.0f),
    m_deleteOnCollision(false),
    m_projSpeed( 10.0f ),
    m_fireRate(0.2f),
    m_fireTimer(0.0f),
    m_reloadTime(0.0f),
    m_reloadTimer(0.0f),
    m_recoilAngle(10),
    m_maxRange(10.0f),
    m_accuracy( 1.0f ),
    m_spreadFactor( 1.0f ),
    m_maxAmmoCount(0),
    m_clipSize(0),
    m_projFireCount(1),
    m_weaponType(PRIMARY_WEAPON),
    m_camHandle(nullptr),
    m_firePosHandle(nullptr),
    m_archetypeToShoot("BaseBullet"),
    m_triggerPulled(false),
    m_active(true)
{   
    m_ammoCount = m_maxAmmoCount;
    m_clipCount = m_clipSize;
}

AbstractWeapon::~AbstractWeapon(void)
{
}

void AbstractWeapon::Initialize(ursine::ecs::Entity* owner)
{
    m_owner = owner;

    if ( m_maxAmmoCount == 0 )
        m_maxAmmoCount = UNLIMITED_AMMO;

    if ( m_clipSize == 0 )
        m_clipSize = UNLIMITED_CLIP;

    m_ammoCount = m_maxAmmoCount;
    m_clipCount = m_clipSize;
}

int AbstractWeapon::FireLogic(void)
{
    return 1;
}

///////////////////////////////
////  Gettors and Settors  ////
///////////////////////////////

int AbstractWeapon::CanFire(void) const
{
    // check fire timer
    if ( m_fireTimer > 0.0f )
        return FIRE_TIMER_SET;

    // check if reloading
    if ( m_reloadTimer > 0.0f )
        return RELOAD_IN_PROCESS;

    // check clip
    if ( m_clipCount == 0 && m_clipSize != UNLIMITED_CLIP )
        return MUST_RELOAD;

    // check if trigger pulled
    if ( !m_triggerPulled )
        return TRIGGER_NOT_PULLED;

    return CAN_FIRE;
}

//// Damage
float AbstractWeapon::GetDamageToApply(void) const
{
    return m_damageToApply;
}

void AbstractWeapon::SetDamageToApply(const float damage)
{
    m_damageToApply = damage;
}

float AbstractWeapon::GetCritModifier(void) const
{
    return m_critModifier;
}

void AbstractWeapon::SetCritModifier(const float modifier)
{
    m_critModifier = modifier;
}

float AbstractWeapon::GetDamageInterval(void) const
{
    return m_damageInterval;
}

void AbstractWeapon::SetDamageInterval(const float damageInterval)
{
    m_damageInterval = damageInterval;
}

bool AbstractWeapon::GetDeleteOnCollision(void) const
{
    return m_deleteOnCollision;
}

float AbstractWeapon::GetProjSpeed(void) const
{
    return m_projSpeed;
}

void AbstractWeapon::SetProjSpeed(const float speed)
{
    m_projSpeed = speed;
}

void AbstractWeapon::SetDeleteOnCollision(const bool state)
{
    m_deleteOnCollision = state;
}


// Fire Rate
float AbstractWeapon::GetFireRate(void) const
{
    return m_fireRate;
}

void AbstractWeapon::SetFireRate(const float rate)
{
    m_fireRate = rate;
}

float AbstractWeapon::GetReloadTime(void) const
{
    return m_reloadTime;
}

void AbstractWeapon::SetReloadTime(const float time)
{
    m_reloadTime = time;
}


// Recoil Angle
float AbstractWeapon::GetRecoilAngle(void) const
{
    return m_recoilAngle;
}

void AbstractWeapon::SetRecoilAngle(const float angle)
{
    m_recoilAngle = angle;
}


// Max Range
float AbstractWeapon::GetMaxRange(void) const
{
    return m_maxRange;
}

void AbstractWeapon::SetMaxRange(const float range)
{
    m_maxRange = range;
}


// accuracy
float AbstractWeapon::GetAccuracy(void) const
{
    return m_accuracy;
}

void AbstractWeapon::SetAccuracy(const float accuracy)
{
    m_accuracy = clamp(0.0f, 1.0f, accuracy);
}


// Spread moefoe
float AbstractWeapon::GetSpreadFactor(void) const
{
    return m_spreadFactor;
}

void AbstractWeapon::SetSpreadFactor(const float spread)
{
    m_spreadFactor = spread;
}

// Ammo Count
int AbstractWeapon::GetAmmoCount(void) const
{
    return m_ammoCount;
}

void AbstractWeapon::SetAmmoCount(const int ammo)
{
    m_ammoCount = ammo;
}


// Max Ammo Count
int AbstractWeapon::GetMaxAmmoCount(void) const
{
    return m_maxAmmoCount;
}

void AbstractWeapon::SetMaxAmmoCount(const int maxAmmo)
{
    m_maxAmmoCount = maxAmmo;

    if ( m_maxAmmoCount == 0 )
    {
        m_maxAmmoCount = UNLIMITED_AMMO;
        m_ammoCount = m_maxAmmoCount;
    }

    else if ( m_ammoCount > m_maxAmmoCount )
        m_ammoCount = m_maxAmmoCount;
}


// Clip Count
int AbstractWeapon::GetClipCount(void) const
{
    return m_clipCount;
}

void AbstractWeapon::SetClipCount(const int count)
{
    m_clipCount = count;
}


// Clip Size
int AbstractWeapon::GetClipSize(void) const
{
    return m_clipSize;
}

void AbstractWeapon::SetClipSize(const int size)
{
    m_clipSize = size;

    if ( m_clipSize == 0 )
    {
        m_clipSize = UNLIMITED_AMMO;
        m_clipCount = m_clipSize;
    }

    else if ( m_clipCount > m_clipSize )
        m_clipCount = m_clipSize;
}


// Projectile fire count
int AbstractWeapon::GetProjFireCount( ) const
{
    return m_projFireCount;
}

void AbstractWeapon::SetProjFireCount(const int count)
{
    m_projFireCount = count;
}


// Archetype To Shoot
const std::string& AbstractWeapon::GetArchetypeToShoot(void) const
{
    return m_archetypeToShoot;
}

// Helper to check if the archetype to shoot needs to have .uatype appended to it
void CheckArchetypeToShoot(std::string& archetype)
{
    if ( archetype.find(".uatype") == std::string::npos )
        archetype += ".uatype";
}

void AbstractWeapon::SetArchetypeToShoot(const char * archetype)
{
    m_archetypeToShoot = archetype;

    CheckArchetypeToShoot(m_archetypeToShoot);
}

// weapon type
WeaponType AbstractWeapon::GetWeaponType( ) const
{
    return m_weaponType;
}

void AbstractWeapon::SetWeaponType(const WeaponType type)
{
    m_weaponType = type;
}


// spawn offset
ursine::SVec3 AbstractWeapon::GetSpawnOffset(void) const
{
    return m_spawnOffset;
}

void AbstractWeapon::SetSpawnOffset(const ursine::SVec3& offset)
{
    if ( m_owner )
    {
        ursine::ecs::Transform* trans = m_owner->GetTransform( );
    
        trans->SetLocalPosition(m_owner->GetTransform( )->GetLocalPosition( ) - ( trans->GetLocalRotation( ) * m_spawnOffset ));

        m_spawnOffset = offset;

        trans->SetLocalPosition(m_owner->GetTransform( )->GetLocalPosition( ) + ( trans->GetLocalRotation( ) * m_spawnOffset ));
    }

    m_spawnOffset = offset;
}


void AbstractWeapon::SetArchetypeToShoot(const std::string& archetype)
{
    m_archetypeToShoot = archetype;

    CheckArchetypeToShoot(m_archetypeToShoot);
}


// Add ammo
bool AbstractWeapon::AddAmmo(const int ammo)
{
    if ( m_ammoCount == m_maxAmmoCount )
        return MAX_AMMO;

    m_ammoCount = m_ammoCount + ammo;

    if ( m_ammoCount > m_maxAmmoCount )
        m_ammoCount = m_maxAmmoCount;

    return AMMO_USED;
}



// Trigger Pulled
bool AbstractWeapon::GetTriggerPulled(void) const
{
    return m_triggerPulled;
}

void AbstractWeapon::TriggerPulled( EVENT_HANDLER( game::FIRE_START ) )
{
    m_triggerPulled = true;
}

void AbstractWeapon::TriggerReleased( EVENT_HANDLER( game::FIRE_END ) )
{
    m_triggerPulled = false;
}


void AbstractWeapon::ActivateWeapon(EVENT_HANDLER(game::ACTIVATE_WEAPON))
{
    EVENT_SENDER(ursine::ecs::Entity, sender);

    game::WeaponActivationEventArgs* args = static_cast<game::WeaponActivationEventArgs*>( const_cast<ursine::EventArgs*>( _args ) );

    // connect to parent's fire event
    args->whoToConnect->Listener(this)
        .On(game::FIRE_START, &AbstractWeapon::TriggerPulled)
        .On(game::FIRE_END, &AbstractWeapon::TriggerReleased);

    // Gun is being reloaded from inventory (swapped in) so update
    //   ammo and clip to previous values before swapped out
    if ( args->m_ammo != -1 )
    {
        SetAmmoCount( args->m_ammo );
        SetClipCount( args->m_clip );
    }

    // Grab camera handle for shooting
    m_camHandle = *args->m_camHandle;

    // Give access to spawn offset
    args->m_spawnOffset = &m_spawnOffset;

    // Grab fire position child
    ursine::ecs::Entity* firePos = sender->GetChildByName("FirePos");

    // if the fire position was a child then grab transform for shooting
    if ( firePos )
        m_firePosHandle = firePos->GetTransform( );

    // Grab animator of weapon's child model
    m_animatorHandle = sender->GetComponentInChildren<ursine::ecs::Animator>( );
}

void AbstractWeapon::DetachWeapon(EVENT_HANDLER(game::DETACH_WEAPON))
{
    EVENT_ATTRS(ursine::ecs::Entity, game::WeaponDeactivationEventArgs);

    // disconnect from parent's fire event
    args->whoToConnect->Listener(this)
        .Off(game::FIRE_START, &AbstractWeapon::TriggerPulled)
        .Off(game::FIRE_END, &AbstractWeapon::TriggerReleased);

    // unattach from parent
    sender->GetTransform( )->DetachFromParent( );

    // give rigidbody so fall to the ground
    ursine::ecs::Rigidbody* body = sender->AddComponent<ursine::ecs::Rigidbody>( );

    // give body an impulse to simulate throwing
    body->AddImpulse( sender->GetTransform( )->GetForward( ) * 3 + ursine::SVec3(0.0f, 3.0f, 0.0f) );

    // need dat collision for floor
    sender->AddComponent<ursine::ecs::BoxCollider>( );

    // need dat interactability
    sender->AddComponent<Interactable>( );

    // make a weapon pick up
    WeaponPickup* pickup = sender->AddComponent<WeaponPickup>( );
    pickup->SetAmmoInfo( m_ammoCount, m_clipCount );

    // flag to remove this component
    RemoveMySelf( );
}

void AbstractWeapon::DeactivateWeapon(EVENT_HANDLER(game::DEACTIVATE_WEAPON))
{
    game::WeaponDeactivationEventArgs* args = static_cast<game::WeaponDeactivationEventArgs*>( const_cast<ursine::EventArgs*>(_args) );

    // disconnect from parent's fire event
    args->whoToConnect->Listener(this)
        .Off(game::FIRE_START, &AbstractWeapon::TriggerPulled)
        .Off(game::FIRE_END, &AbstractWeapon::TriggerReleased);

    args->m_ammo = m_ammoCount;
    args->m_clip = m_clipCount;
}


void AbstractWeapon::PickUpAmmo(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED))
{
    EVENT_ATTRS(ursine::ecs::Entity, ursine::physics::CollisionEventArgs);

    if ( args->otherEntity->HasComponent<AmmoPickup>( ) )
    {
        AmmoPickup& pickup = *args->otherEntity->GetComponent<AmmoPickup>( );

        if ( AddAmmo( args->otherEntity->GetComponent<AmmoPickup>( )->m_count ) )
        {
            args->otherEntity->Delete( );
        }
    }
}




