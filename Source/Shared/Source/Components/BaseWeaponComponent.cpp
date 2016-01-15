#include "Precompiled.h"
#include "BaseWeaponComponent.h"
#include "GameEvents\GamePlayEvents.h"

NATIVE_COMPONENT_DEFINITION( BaseWeapon );

using namespace ursine;

#define UNLIMITED_AMMO  INFINITE
#define UNLIMITED_CLIP  INFINITE

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
    m_fireRate( 0.2f ),
    m_fireTimer( 0.0f ),
    m_reloadTime( 0.0f ),
    m_reloadTimer( 0.0f ),
    m_recoilAngle( 10 ),
    m_maxRange( 0 ),
    m_maxAmmoCount( 0 ),
    m_clipSize( 0 ),
    m_shotType( Single_Shot ),
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
        .Off( gameEvent::FIRE_START, &BaseWeapon::TriggerPulled )
        .Off( gameEvent::FIRE_END, &BaseWeapon::TriggerReleased );
}

void BaseWeapon::OnInitialize( void )
{
    GetOwner( )->Listener( this )
        .On( gameEvent::FIRE_START, &BaseWeapon::TriggerPulled )
        .On( gameEvent::FIRE_END, &BaseWeapon::TriggerReleased );

    if ( m_maxAmmoCount == 0 )
        m_maxAmmoCount = UNLIMITED_AMMO;

    if ( m_clipSize == 0 )
        m_clipSize = UNLIMITED_CLIP;

    m_ammoCount = m_maxAmmoCount;
    m_clipCount = m_clipSize;
}


///////////////////////////////
////  Gettors and Settors  ////
///////////////////////////////

//// Damage
//float BaseWeapon::GetDamage(void)
//{
//    return m_damage;
//}
//
//void BaseWeapon::SetDamage(const float damage)
//{
//    m_damage = damage;
//}
//
//
//// Crit Modifier
//float BaseWeapon::GetCritModifier(void)
//{
//    return m_critModifier;
//}
//
//void BaseWeapon::SetCritModifier(const float modifier)
//{
//    m_critModifier = modifier;
//}
//
//
//// Crit Chance
//float BaseWeapon::GetCritChance(void)
//{
//    return m_critChance;
//}
//
//void BaseWeapon::SetCritChance(const float critChance)
//{
//    m_critChance = critChance;
//}


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
}


// Shot Type
ShotType BaseWeapon::GetShotType( void ) const
{
    return m_shotType;
}

void BaseWeapon::SetShotType( const ShotType type )
{
    m_shotType = type;
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

void BaseWeapon::SetArchetypeToShoot( const char * archetype )
{
    m_archetypeToShoot = archetype;
    m_archetypeToShoot += ".uatype";
}

void BaseWeapon::SetArchetypeToShoot( const std::string& archetype )
{
    m_archetypeToShoot = archetype + ".uatype";
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
    int roundsShot = 0;

    // fire shot type
    switch ( m_shotType )
    {
    case Single_Shot:
        roundsShot = RemoveRoundsFromClip( 1 );
        break;
    case Two_Burst:
        roundsShot = RemoveRoundsFromClip( 2 );
        break;
    case Triple_Burst:
        roundsShot = RemoveRoundsFromClip( 3 );
        break;
    case Quad_Burst:
        roundsShot = RemoveRoundsFromClip( 4 );
        break;
    case Full_Clip:
        roundsShot = RemoveRoundsFromClip( m_clipSize );
        break;
    case Random_Burst:
    {
        // get random number 1 - 5
        int randomNum = getRandomNum( 5 );

        // anything that is not full clip
        if ( randomNum != 5 )
            roundsShot = RemoveRoundsFromClip( randomNum );

        // shoot full clip
        else
            roundsShot = RemoveRoundsFromClip( m_clipSize );

        break;
    }
    default:
        roundsShot = RemoveRoundsFromClip( 1 );
        break;
    }

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

void BaseWeapon::TriggerPulled( EVENT_HANDLER( gameEvent::FIRE_START ) )
{
    m_triggerPulled = true;
}

void BaseWeapon::TriggerReleased( EVENT_HANDLER( gameEvent::FIRE_END ) )
{
    m_triggerPulled = false;
}


// Tries to remove the number of rounds specified from the clip
//   and returns the actual number of rounds removed
int BaseWeapon::RemoveRoundsFromClip( int roundCount )
{
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
        roundsRemoved = -m_clipCount;
        m_clipCount = 0;
    }

    return roundsRemoved;
}

bool BaseWeapon::OutofAmmo( void )
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

