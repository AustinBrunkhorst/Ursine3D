
#include "Precompiled.h"
#include "AbstractWeapon.h"
#include <CollisionEventArgs.h>
#include "AmmoPickupComponent.h"
#include "GameEvents.h"

using namespace ursine;

namespace
{
    float clamp(float min, float max, float val)
    {
        return ( min > val ) ? min : ( ( max < val ) ? max : val );
    }
}


AbstractWeapon::AbstractWeapon(void) :
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
    m_maxRange(0),
    m_accuracy( 1.0f ),
    m_maxAmmoCount(0),
    m_clipSize(0),
    m_projFireCount(1),
    m_weaponFireType(PROJECTILE_WEAPON),
    m_archetypeToShoot("BaseBullet"),
    m_triggerPulled(false)
{   
    m_ammoCount = m_maxAmmoCount;
    m_clipCount = m_clipSize;
}

AbstractWeapon::~AbstractWeapon(void)
{
}

void AbstractWeapon::Initialize(void)
{
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
    m_spreadFactor = clamp(0.0f, 1.0f, spread);
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
