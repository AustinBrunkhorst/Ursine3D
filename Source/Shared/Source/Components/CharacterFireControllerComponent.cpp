#include "Precompiled.h"
#include "CharacterFireControllerComponent.h"

NATIVE_COMPONENT_DEFINITION(CharacterFireController);

CharacterFireController::CharacterFireController( void )
    : BaseComponent( )
    , m_fireRate( 0.1 )
    , m_fireRange( 1000.0f )
    , m_fireOffset( ursine::SVec3( 0, 0, 0 ) )
    , m_triggerFire( false )
    , m_fireTimer( 0.0f )
    , m_canFire( false )
    , m_accuracy( 0 )
    , m_damage( 10 )
    , m_shotStartColor( ursine::Color( 1, 0, 0, 1 ) )
    , m_shotEndColor( ursine::Color( 0, 1, 0.5, 1 ) )
{

}

float CharacterFireController::GetFireRate(void) const
{
    return m_fireRate;
}

void CharacterFireController::SetFireRate(const float fireRate)
{
    m_fireRate = fireRate;
}

void CharacterFireController::Fire(void)
{
    m_fireTimer = m_fireRate;
    m_canFire = false;
}

void CharacterFireController::DecrementFireTimer(const float dt)
{
    m_fireTimer -= dt;
}

void CharacterFireController::SetFireState(const bool fireState)
{
    m_canFire = fireState;
}

bool CharacterFireController::CanFire(void) const
{
    if ( m_triggerFire )
        return m_canFire;

    return m_fireTimer <= 0;
}

float CharacterFireController::GetFireRange(void) const
{
    return m_fireRange;
}

void CharacterFireController::SetFireRange(const float range)
{
    m_fireRange = range;
}

const ursine::SVec3 &CharacterFireController::GetFireOffset(void) const
{
    return m_fireOffset;
}

void CharacterFireController::SetFireOffset(const ursine::SVec3 &offset)
{
    m_fireOffset = offset;
}

bool CharacterFireController::GetTriggerFire(void) const
{
    return m_triggerFire;
}

void CharacterFireController::SetTriggerFire(const bool useTriggerFire)
{
    m_triggerFire = useTriggerFire;
}

ursine::Color CharacterFireController::GetShotStartColor(void) const
{
    return m_shotStartColor;
}

void CharacterFireController::SetShotStartColor(const ursine::Color& shotColor)
{
    m_shotStartColor = shotColor;
}

ursine::Color CharacterFireController::GetShotEndColor( ) const
{
    return m_shotEndColor;
}

void CharacterFireController::SetShotEndColor( const ursine::Color& shotColor )
{
    m_shotEndColor = shotColor;
}

float CharacterFireController::GetAccuracy() const
{
    return m_accuracy;
}

void CharacterFireController::SetAccuracy(const float accuracy)
{
    m_accuracy = accuracy;
}

float CharacterFireController::GetDamage() const
{
    return m_damage;
}

void CharacterFireController::SetDamage(const float damage)
{
    m_damage = damage;
}