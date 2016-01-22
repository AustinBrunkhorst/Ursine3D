#include "Precompiled.h"
#include "BaseWeaponComponent.h"
#include "GameEvents.h"

NATIVE_COMPONENT_DEFINITION( BaseWeapon );

using namespace ursine;

#define UNLIMITED_AMMO  MAXINT
#define UNLIMITED_CLIP  MAXINT

#define RELOAD_SUCCESS  true

#define MAX_AMMO false
#define AMMO_USED true



// return number from 1 to number passed
int getRandomNum( int num )
{
    int randomNum = rand( );
    randomNum = randomNum % 5 + 1;

    return randomNum;
}


BaseWeapon::BaseWeapon( void ) :
    BaseComponent( ),
    m_damageToApply( 1.0f ),
    m_critModifier( 1.0f ),
    m_damageInterval( 1.0f ),
    m_deleteOnCollision( false ),
    m_fireRate( 0.2f ),
    m_fireTimer( 0.0f ),
    m_reloadTime( 0.0f ),
    m_reloadTimer( 0.0f ),
    m_recoilAngle( 10 ),
    m_maxRange( 0 ),
    m_maxAmmoCount( 0 ),
    m_clipSize( 0 ),
    m_projFireCount( 1 ),
    m_aoeType( None ),
    m_archetypeToShoot( "BaseBullet" ),
    m_triggerPulled( false )
{
    m_ammoCount = m_maxAmmoCount;
    m_clipCount = m_clipSize;
}

BaseWeapon::~BaseWeapon( void )
{
    GetOwner( )->Listener( this )
        .Off( game::FIRE_START, &BaseWeapon::TriggerPulled )
        .Off( game::FIRE_END, &BaseWeapon::TriggerReleased );
}

void BaseWeapon::OnInitialize( void )
{
    GetOwner( )->Listener( this )
        .On( game::FIRE_START, &BaseWeapon::TriggerPulled )
        .On( game::FIRE_END, &BaseWeapon::TriggerReleased );

    if ( m_maxAmmoCount == UNLIMITED_AMMO )
        m_maxAmmoCount = UNLIMITED_AMMO;

    if ( m_clipSize == UNLIMITED_CLIP )
        m_clipSize = UNLIMITED_CLIP;

    m_ammoCount = m_maxAmmoCount;
    m_clipCount = m_clipSize;
}


///////////////////////////////
////  Gettors and Settors  ////
///////////////////////////////

//// Damage
float BaseWeapon::GetDamageToApply(void) const
{
    return m_damageToApply;
}

void BaseWeapon::SetDamageToApply(const float damage)
{
    m_damageToApply = damage;
}

float BaseWeapon::GetCritModifier(void) const
{
    return m_critModifier;
}

void BaseWeapon::SetCritModifier(const float modifier)
{
    m_critModifier = modifier;
}

float BaseWeapon::GetDamageInterval(void) const
{
    return m_damageInterval;
}

void BaseWeapon::SetDamageInterval(const float damageInterval)
{
    m_damageInterval = damageInterval;
}

bool BaseWeapon::GetDeleteOnCollision(void) const
{
    return m_deleteOnCollision;
}

float BaseWeapon::GetProjSpeed(void) const
{
    return m_projSpeed;
}

void BaseWeapon::SetProjSpeed(const float speed)
{
    m_projSpeed = speed;
}

void BaseWeapon::SetDeleteOnCollision(const bool state)
{
    m_deleteOnCollision = state;
}


// Fire Rate
float BaseWeapon::GetFireRate( void ) const
{
    return m_fireRate;
}

void BaseWeapon::SetFireRate( const float rate )
{
    m_fireRate = rate;
}

float BaseWeapon::GetReloadTime( void ) const
{
    return m_reloadTime;
}

void BaseWeapon::SetReloadTime( const float time )
{
    m_reloadTime = time;
}


// Recoil Angle
float BaseWeapon::GetRecoilAngle( void ) const
{
    return m_recoilAngle;
}

void BaseWeapon::SetRecoilAngle( const float angle )
{
    m_recoilAngle = angle;
}


// Max Range
float BaseWeapon::GetMaxRange( void ) const
{
    return m_maxRange;
}

void BaseWeapon::SetMaxRange( const float range )
{
    m_maxRange = range;
}


// Ammo Count
int BaseWeapon::GetAmmoCount( void ) const
{
    return m_ammoCount;
}

void BaseWeapon::SetAmmoCount( const int ammo )
{
    m_ammoCount = ammo;
}


// Max Ammo Count
int BaseWeapon::GetMaxAmmoCount( void ) const
{
    return m_maxAmmoCount;
}

void BaseWeapon::SetMaxAmmoCount( const int maxAmmo )
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
int BaseWeapon::GetClipCount( void ) const
{
    return m_clipCount;
}

void BaseWeapon::SetClipCount( const int count )
{
    m_clipCount = count;
}


// Clip Size
int BaseWeapon::GetClipSize( void ) const
{
    return m_clipSize;
}

void BaseWeapon::SetClipSize( const int size )
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
int BaseWeapon::GetProjFireCount( ) const
{
    return m_projFireCount;
}

void BaseWeapon::SetProjFireCount(const int count)
{
    m_projFireCount = count;
}


// AOE Type
AOEType BaseWeapon::GetAOEType( void ) const
{
    return m_aoeType;
}

void BaseWeapon::SetAOEType( const AOEType type )
{
    m_aoeType = type;
}


// Archetype To Shoot
const std::string& BaseWeapon::GetArchetypeToShoot( void ) const
{
    return m_archetypeToShoot;
}

// Helper to check if the archetype to shoot needs to have .uatype appended to it
void CheckArchetypeToShoot(std::string& archetype)
{
    if ( archetype.find( ".uatype" ) == std::string::npos )
        archetype += ".uatype";
}

void BaseWeapon::SetArchetypeToShoot( const char * archetype )
{
    m_archetypeToShoot = archetype;

    CheckArchetypeToShoot( m_archetypeToShoot );
}

void BaseWeapon::SetArchetypeToShoot( const std::string& archetype )
{
    m_archetypeToShoot = archetype;

    CheckArchetypeToShoot( m_archetypeToShoot );
}


// Trigger Pulled
bool BaseWeapon::GetTriggerPulled( void ) const
{
    return m_triggerPulled;
}



//////////////////////
////  Fire Logic  ////
//////////////////////

int BaseWeapon::CanFire( void ) const
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

int BaseWeapon::Fire( void )
{
    m_fireTimer = m_fireRate;

    // number of rounds that were fired
    int roundsShot = RemoveRoundsFromClip( m_projFireCount );

    printf( "Rounds Shot: %d \n Ammo: %d\n Clip: %d\n", roundsShot, m_ammoCount, m_clipCount );

    return roundsShot;
}


////////////////////////
////  Weapon Logic  ////
////////////////////////

void BaseWeapon::DecrementFireTimer( const float dt )
{
        m_fireTimer -= dt;
}

void BaseWeapon::DecrementReloadTimer( const float dt )
{
    m_reloadTimer -= dt;
}

bool BaseWeapon::AddAmmo( const int ammo )
{
    if ( m_ammoCount == m_maxAmmoCount )
        return MAX_AMMO;

    m_ammoCount = m_ammoCount + ammo;

    if ( m_ammoCount > m_maxAmmoCount )
        m_ammoCount = m_maxAmmoCount;

    return AMMO_USED;
}

int BaseWeapon::Reload( void )
{
    // Is weapon out of ammo?
    if ( OutofAmmo( ) )
        return NO_AMMO;

    // is weapon's clip full
    if ( ClipFull( ) )
        return CLIP_FULL;

    if ( m_maxAmmoCount == UNLIMITED_AMMO )
        m_clipCount = m_clipSize;

    // reset clip to max
    else if ( m_ammoCount > m_clipSize )
    {
        m_clipCount = m_clipSize;
        m_ammoCount -= m_clipSize;
    }

    // not enough ammo to reload fully
    else
    {
        m_clipCount = m_ammoCount;
        m_ammoCount = 0;
    }

    // set reload timer
    m_reloadTimer = m_reloadTime;

    return RELOAD_SUCCESS;
}

void BaseWeapon::TriggerPulled( EVENT_HANDLER( game::FIRE_START ) )
{
    m_triggerPulled = true;
}

void BaseWeapon::TriggerReleased( EVENT_HANDLER( game::FIRE_END ) )
{
    m_triggerPulled = false;
}


// Tries to remove the number of rounds specified from the clip
//   and returns the actual number of rounds removed
int BaseWeapon::RemoveRoundsFromClip( int roundCount )
{
    URSINE_TODO( "check for unlimited clip" );

    // only shoot 5 if clip is unlimited
    if ( roundCount == UNLIMITED_CLIP )
        return 5;

    // how many rounds were removed
    int roundsRemoved = roundCount;

    // remove rounds
    m_clipCount -= roundCount;

    // if rounds removed were more than amount in clip
    if ( m_clipCount < 0 )
    {
        // calculating actual count removed
        roundsRemoved += m_clipCount;
        m_clipCount = 0;
    }

    return roundsRemoved;
}

bool BaseWeapon::OutofAmmo( void ) const
{
    // can't reload if no ammo or clip is full
    if ( m_ammoCount == 0 )
        return true;

    return false;
}

bool BaseWeapon::ClipFull( void )
{
    return m_clipCount == m_clipSize;
}

