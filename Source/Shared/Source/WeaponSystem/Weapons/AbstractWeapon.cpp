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
#include <FirePosComponent.h>

using namespace ursine;

#define ARBITRARY_NUM 100.0f

AbstractWeapon::AbstractWeapon(void) 
    : m_owner( )
    , m_fireRate( 0.2f )
    , m_fireTimer( 0.0f )
    , m_reloadTime( 0.0f )
    , m_reloadTimer( 0.0f )
    , m_recoilAngle( 10 )
    , m_maxRange( 10.0f )
    , m_accuracy( 1.0f )
    , m_spread( -1.0f, 1.0f )
    , m_maxAmmoCount( 0 )
    , m_clipSize( 0 )
    , m_projFireCount( 1 )
    , m_weaponType( PRIMARY_WEAPON )
    , m_camHandle( nullptr )
    , m_firePosHandle( nullptr )
    , m_semiAutomatic( false )
    , m_triggerPulled( false )
    , m_active( false )
{   
    m_ammoCount = m_maxAmmoCount;
    m_clipCount = m_clipSize;
}

AbstractWeapon::~AbstractWeapon(void)
{
    m_owner->Listener(this)
        .Off(game::ACTIVATE_WEAPON, &AbstractWeapon::ActivateWeapon)
        .Off(game::DETACH_WEAPON, &AbstractWeapon::DetachWeapon)
        .Off(game::DEACTIVATE_WEAPON, &AbstractWeapon::DeactivateWeapon);
}

void AbstractWeapon::Initialize(const ecs::EntityHandle &owner)
{
    m_owner = owner;

    if ( m_maxAmmoCount == 0 )
        m_maxAmmoCount = UNLIMITED_AMMO;

    if ( m_clipSize == 0 )
        m_clipSize = UNLIMITED_CLIP;

    m_ammoCount = m_maxAmmoCount;
    m_clipCount = m_clipSize;

    m_owner->Listener(this)
        .On(game::ACTIVATE_WEAPON, &AbstractWeapon::ActivateWeapon)
        .On(game::DETACH_WEAPON, &AbstractWeapon::DetachWeapon)
        .On(game::DEACTIVATE_WEAPON, &AbstractWeapon::DeactivateWeapon);
}

void AbstractWeapon::ConnectTrigger(const ursine::ecs::EntityHandle &obj)
{
    obj->Listener(this)
        .On(game::FIRE_START, &AbstractWeapon::TriggerPulled)
        .On(game::FIRE_END, &AbstractWeapon::TriggerReleased);
}

int AbstractWeapon::FireLogic(void)
{
    return 1;
}

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

///////////////////////////////
////  Gettors and Settors  ////
///////////////////////////////

// Fire Rate
float AbstractWeapon::GetFireRate(void) const
{
    return m_fireRate;
}

void AbstractWeapon::SetFireRate(float rate)
{
    m_fireRate = rate;
}

// Reload time
float AbstractWeapon::GetReloadTime(void) const
{
    return m_reloadTime;
}

void AbstractWeapon::SetReloadTime(float time)
{
    m_reloadTime = time;
}

// Recoil Angle
float AbstractWeapon::GetRecoilAngle(void) const
{
    return m_recoilAngle;
}

void AbstractWeapon::SetRecoilAngle(float angle)
{
    m_recoilAngle = angle;
}

// Max Range
float AbstractWeapon::GetMaxRange(void) const
{
    return m_maxRange;
}

void AbstractWeapon::SetMaxRange(float range)
{
    m_maxRange = range;
}

// accuracy
float AbstractWeapon::GetAccuracy(void) const
{
    return m_accuracy;
}

void AbstractWeapon::SetAccuracy(float accuracy)
{
    m_accuracy = math::Clamp( 0.0f, 1.0f, accuracy );
}

// Spread moefoe
float AbstractWeapon::GetSpreadFactor(void) const
{
    return m_spread.GetMax( );
}

void AbstractWeapon::SetSpreadFactor(float spread)
{
    m_spread.SetMax(spread);
    m_spread.SetMin(-spread);
}

// Ammo Count
int AbstractWeapon::GetAmmoCount(void) const
{
    return m_ammoCount;
}

void AbstractWeapon::SetAmmoCount(int ammo)
{
    m_ammoCount = ammo;
}

// Max Ammo Count
int AbstractWeapon::GetMaxAmmoCount(void) const
{
    return m_maxAmmoCount;
}

void AbstractWeapon::SetMaxAmmoCount(int maxAmmo)
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

void AbstractWeapon::SetClipCount(int count)
{
    m_clipCount = count;
}

// Clip Size
int AbstractWeapon::GetClipSize(void) const
{
    return m_clipSize;
}

void AbstractWeapon::SetClipSize(int size)
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
int AbstractWeapon::GetProjFireCount(void) const
{
    return m_projFireCount;
}

void AbstractWeapon::SetProjFireCount(int count)
{
    m_projFireCount = count;
}

// weapon type
WeaponType AbstractWeapon::GetWeaponType(void) const
{
    return m_weaponType;
}

void AbstractWeapon::SetWeaponType(WeaponType type)
{
    m_weaponType = type;
}

// particle to use when firing on muzzle area
const ursine::resources::ResourceReference& AbstractWeapon::GetFireParticle(void) const
{
    return m_fireParticle;
}

void AbstractWeapon::SetFireParticle(const ursine::resources::ResourceReference& archetype)
{
    m_fireParticle = archetype;
}

// semi automatic
bool AbstractWeapon::GetSemiAutomatic(void) const
{
    return m_semiAutomatic;
}

void AbstractWeapon::SetSemiAutomatic(bool semi)
{
    m_semiAutomatic = semi;

    if ( semi && m_fireRate == 0 )
        m_fireRate = ARBITRARY_NUM;
}

// Add ammo
bool AbstractWeapon::AddAmmo(int ammo)
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

void AbstractWeapon::TriggerPulled(EVENT_HANDLER(game::FIRE_START))
{
    m_triggerPulled = true;
}

void AbstractWeapon::TriggerReleased( EVENT_HANDLER(game::FIRE_END))
{
    m_triggerPulled = false;

    // reset fire timer to be able to shoot again
    if (m_semiAutomatic)
        m_fireTimer = 0.0f;
}

void AbstractWeapon::ActivateWeapon(const ecs::EntityHandle &whoToConnect, ecs::Transform* camHandle, int ammo, int clip)
{
    // allow firing
    m_active = true;
    
    // who will be telling me to shoot
    m_boss = whoToConnect;

    // connect to parent's fire event
    m_boss->Listener( this )
        .On( game::FIRE_START, &AbstractWeapon::TriggerPulled )
        .On( game::FIRE_END, &AbstractWeapon::TriggerReleased )
        .On(game::CEASE_FIRE, &AbstractWeapon::CeaseFire)
        .On(game::FIRE_AT_WILL, &AbstractWeapon::FireAtWill);

    // Gun is being reloaded from inventory (swapped in) so update
    //   ammo and clip to previous values before swapped out
    if (ammo != -1)
    {
        SetAmmoCount( ammo );
        SetClipCount( clip );
    }

    // Grab camera handle for shooting
    if ( camHandle )
        m_camHandle = camHandle;
    else
        m_camHandle = m_owner->GetTransform( );

    // Grab fire position child
    FirePos* firePos = m_owner->GetComponentInChildren<FirePos>( );

    // if the fire position was a child then grab transform for shooting
    if (firePos)
        m_firePosHandle = firePos->GetOwner( )->GetTransform( );
    else
        m_firePosHandle = m_owner->GetTransform( );

    // Grab animator of weapon's child model
    m_animatorHandle = m_owner->GetComponentInChildren<ecs::Animator>( );
}

void AbstractWeapon::DetachWeapon(void)
{
    // disable firing
    m_active = false;

    // disconnect from parent's fire event
    m_boss->Listener( this )
        .Off( game::FIRE_START, &AbstractWeapon::TriggerPulled )
        .Off( game::FIRE_END, &AbstractWeapon::TriggerReleased )
        .Off(game::CEASE_FIRE, &AbstractWeapon::CeaseFire)
        .Off(game::FIRE_AT_WILL, &AbstractWeapon::FireAtWill);

    // unattach from parent
    m_owner->GetTransform( )->DetachFromParent( );

    // give rigidbody so fall to the ground
    ecs::Rigidbody* body = m_owner->AddComponent<ecs::Rigidbody>( );

    // give body an impulse to simulate throwing
    body->AddImpulse(m_owner->GetTransform( )->GetForward( ) * 3 + SVec3(0.0f, 3.0f, 0.0f) );

    // need dat collision for floor
    m_owner->AddComponent<ecs::BoxCollider>( );

    // need dat interactability
    m_owner->AddComponent<Interactable>( );

    // make a weapon pick up
    WeaponPickup* pickup = m_owner->AddComponent<WeaponPickup>( );
    pickup->SetAmmoInfo( m_ammoCount, m_clipCount );

    // flag to remove this component
    RemoveMySelf( );
}

void AbstractWeapon::DeactivateWeapon(int& saveAmmo, int& saveClip)
{
    // disconnect from parent's fire event
    m_boss->Listener( this )
        .Off( game::FIRE_START, &AbstractWeapon::TriggerPulled )
        .Off( game::FIRE_END, &AbstractWeapon::TriggerReleased );

    saveAmmo = m_ammoCount;
    saveClip = m_clipCount;
}

void AbstractWeapon::ActivateWeapon(EVENT_HANDLER(game::ACTIVATE_WEAPON))
{
    EVENT_ATTRS(ursine::ecs::Entity, game::WeaponActivationEventArgs);

    ActivateWeapon( args->whoToConnect, args->m_camHandle, args->m_ammo, args->m_clip );
}

void AbstractWeapon::DetachWeapon(EVENT_HANDLER(game::DETACH_WEAPON))
{
    DetachWeapon( );
}

void AbstractWeapon::DeactivateWeapon(EVENT_HANDLER(game::DEACTIVATE_WEAPON))
{
    game::WeaponDeactivationEventArgs* args = static_cast<game::WeaponDeactivationEventArgs*>( const_cast<ursine::EventArgs*>( _args ) );

    DeactivateWeapon( args->m_ammo, args->m_clip );
}

void AbstractWeapon::PickUpAmmo(EVENT_HANDLER(game::PICKUP_AMMO))
{
    EVENT_ATTRS( ursine::ecs::Entity, game::AmmoPickupEventArgs );

    AddAmmo( args->m_ammo );
}

void AbstractWeapon::CeaseFire(EVENT_HANDLER(game::CEASE_FIRE))
{
    m_active = false;
}

void AbstractWeapon::FireAtWill(EVENT_HANDLER(game::FIRE_AT_WILL))
{
    m_active = true;
}