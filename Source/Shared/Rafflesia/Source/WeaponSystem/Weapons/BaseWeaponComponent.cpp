#include "Precompiled.h"
#include "BaseWeaponComponent.h"
#include "GameEvents.h"

NATIVE_COMPONENT_DEFINITION( BaseWeapon );

BaseWeapon::BaseWeapon( void ) :
    BaseComponent( ),
    AbstractProjWeapon( )
{
}

BaseWeapon::~BaseWeapon( void )
{
    AbstractWeaponDisconnect( BaseWeapon );
}

void BaseWeapon::OnInitialize( void )
{
    AbstractWeaponInit( BaseWeapon, GetOwner( ) );
}

void BaseWeapon::RemoveMySelf(void)
{
    GetOwner( )->Delete( );
}
