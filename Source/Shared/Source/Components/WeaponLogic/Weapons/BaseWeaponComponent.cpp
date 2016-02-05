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
    AbstractWeaponDisconnect( BaseWeapon );
}

void BaseWeapon::OnInitialize( void )
{
    AbstractWeaponInit( BaseWeapon );
}

void BaseWeapon::RemoveMySelf(void)
{
    GetOwner( )->Delete( );
}

