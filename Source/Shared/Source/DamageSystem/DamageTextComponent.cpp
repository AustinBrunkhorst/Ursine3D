/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageTextComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "DamageTextComponent.h"
#include "Randomizer.h"
#include "SVec3.h"

NATIVE_COMPONENT_DEFINITION( DamageText ) ;

using namespace ursine;


DamageText::DamageText(void) :
    BaseComponent( ),
    m_lifeTime( 0.0f ),
    m_totalLifeTime( 2.0f ),
    m_velocity(0.0f, 0.0f, 0.0f),
    m_varX(0.0f, 0.0f),
    m_varY(0.0f, 0.0f),
    m_varZ(0.0f, 0.0f)
{
}

DamageText::~DamageText(void)
{
    //GetOwner( )->Listener( this )
    //     .Off( game::FIRE_END, &BaseWeapon::TriggerReleased ); 
}

void DamageText::OnInitialize(void)
{
    m_varX.SetMin( 0.0f );
    m_varY.SetMin( 0.0f );
    m_varZ.SetMin( 0.0f );
}


void DamageText::GenerateVelocity(void)
{
    m_velocity += ursine::SVec3( m_varX.GetValue( ), m_varY.GetValue( ), m_varZ.GetValue( ) );
}

float DamageText::GetTotalLifeTime(void) const
{
    return m_totalLifeTime;
}

void DamageText::SetTotalLifeTime(const float lifeTime)
{
    m_totalLifeTime = lifeTime;
}


// Life Time
float DamageText::GetLifeTime(void) const
{
    return m_lifeTime;
}

void DamageText::IncrementLifeTime(const float lifeTime)
{
    m_lifeTime += lifeTime;
}


// velocity //
const ursine::SVec3& DamageText::GetVelocity(void) const
{
    return m_velocity;
}

void DamageText::SetVelocity(const ursine::SVec3& vel)
{
    m_velocity = vel;
}


// variance //
const ursine::SVec3 DamageText::GetVariance( ) const
{
    return ursine::SVec3( m_varX.GetMax( ), m_varY.GetMax( ), m_varZ.GetMax( ) );
}

void DamageText::SetVariance(const ursine::SVec3& var)
{
    m_varX.SetMax( var.X( ) );
    m_varY.SetMax( var.Y( ) );
    m_varZ.SetMax( var.Z( ) );
}



const float DamageText::GetStartAlpha(void) const
{
    return m_startAlpha;
}

void DamageText::SetStartAlpha(const float alpha)
{
    m_startAlpha = alpha;
}
