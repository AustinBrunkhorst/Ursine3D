#include "Precompiled.h"
#include "BaseWeaponComponent.h"
#include "GameEvents.h"

NATIVE_COMPONENT_DEFINITION( BaseWeapon );

using namespace ursine;


BaseWeapon::BaseWeapon( void ) :
    BaseComponent( ),
    AbstractWeapon( )
{
}

BaseWeapon::~BaseWeapon( void )
{
    GetOwner( )->Listener( this )
        .Off( game::FIRE_START, &BaseWeapon::TriggerPulled )
        .Off( game::FIRE_END, &BaseWeapon::TriggerReleased );
}

void BaseWeapon::OnInitialize( void )
{
    AbstractWeaponInit( BaseWeapon );
}

