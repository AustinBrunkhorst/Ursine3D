#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DisempowerTriggerComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include "MadRaidTrigger.h"



class DisempowerTrigger : public ursine::ecs::Component, public MadRaidTrigger
{
    NATIVE_COMPONENT
public:

    MAD_RAID_TRIGGER_FIELDS( );

    DisempowerTrigger(void);
    ~DisempowerTrigger(void);

protected:
    void OnInitialize(void) override;
    void OnSceneReady(ursine::Scene* scene) override;

    // interaction logic
    void ChildStartInteractionLogic(void) override;
    void ChildStopInteractionLogic(void) override;
    void ChildSuccessLogic(int id) override;
    void ChildUpdate(void);

    void OnEmpowerActive(EVENT_HANDLER(ursine::ecs::Entity));
    void OnEmpowerUnactive(EVENT_HANDLER(ursine::ecs::Entity));

    bool m_empowerActive;

} Meta( Enable, DisplayName( "DisempowerTrigger" ), RequiresComponents( typeof( Interactable ) ) );

