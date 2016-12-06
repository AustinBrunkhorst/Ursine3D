/*---------------------------------------------------------------------------
**Team Bear King
**?2015 DigiPen Institute of Technology, All Rights Reserved.
**
**AttackComponent.cpp
**
**Author:
**- Jordan Ellis - j.ellis@digipen.edu
**
**Contributors:
**- <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "AttackComponent.h"
#include "CoreSystem.h"
#include "KeyboardManager.h"

using namespace ursine;
using namespace ecs;

NATIVE_COMPONENT_DEFINITION(Attack);

Attack::Attack(void)
    : BaseComponent( )
    , m_attacking( false )
    , m_time( 2.5f )
    , m_t( 0.0f ) { }

void Attack::OnInitialize()
{
    GetOwner( )->GetWorld( )->Listener( this ).On(
        WORLD_UPDATE, &Attack::onUpdate
    );
}

void Attack::onUpdate(EVENT_HANDLER(World))
{
    if (GetCoreSystem(KeyboardManager)->IsDown(KEY_SPACE))
    {
        m_attacking = true;
    }

    if (m_attacking)
    {
        m_t += m_time * Application::Instance->GetDeltaTime( );

        m_t = math::Clamp(m_t, 0.0f, 1.0f);

        if (math::IsEqual(m_t, 1.0f))
        {
            m_attacking = false;
        }
    }
    else
    {
        m_t -= m_time * Application::Instance->GetDeltaTime();

        m_t = math::Clamp(m_t, 0.0f, 1.0f);
    }

    GetOwner( )->GetComponent<IKRoot>( )->SetT( m_t );
}
