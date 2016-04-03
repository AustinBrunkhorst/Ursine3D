/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageTextComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "EntityAnimatorComponent.h"
#include <SpriteTextComponent.h>

class DamageText : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    DamageText(void);

    void OnInitialize(void) override;

private:

    void onAnimationFinish(EVENT_HANDLER(EntityAnimator));

} Meta(
    Enable,
    DisplayName( "DamageText" )
) EditorMeta(
    RequiresComponents( 
        typeof( ursine::ecs::SpriteText ), 
        typeof( EntityAnimator ) 
    )    
);
