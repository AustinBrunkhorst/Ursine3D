/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EmpowerTriggerComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "MadRaidTrigger.h"

class EmpowerTrigger : public ursine::ecs::Component, public MadRaidTrigger
{
    NATIVE_COMPONENT;

public:
    MAD_RAID_TRIGGER_FIELDS( );

    EmpowerTrigger(void);
    ~EmpowerTrigger(void);

protected:
    void OnInitialize(void) override;

    void OnSceneReady(ursine::Scene* scene) override;

    // interaction logic
    void ChildStartInteractionLogic(void) override;
    void ChildStopInteractionLogic(void) override;
    void ChildSuccessLogic(int id) override;
    void ChildUpdate(void) override;


    void OnDisempowerActive(EVENT_HANDLER(ursine::ecs::Entity));
    void OnDisempowerUnactive(EVENT_HANDLER(ursine::ecs::Entity));

    bool m_disempowerActive;
    bool m_playerActive;
} Meta(
    Enable, 
    DisplayName( "EmpowerTrigger" )
) EditorMeta(
    RequiresComponents( typeof( Interactable ) )    
);
