/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialDialogueManagerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "TutorialDialogueManagerComponent.h"

#include "UIEvents.h"
#include "UIScreensConfigComponent.h"

#include <Scene.h>

NATIVE_COMPONENT_DEFINITION( TutorialDialogueManager );

using namespace ursine;
using namespace ecs;

TutorialDialogueManager::TutorialDialogueManager(void)
    : BaseComponent( )
    , cinematicDialogueTimespan( 0.0f )
    , movementTutDialogueTimespan( 0.0f )
    , jumpTutDialogueTimespan( 0.0f )
    , shootingTutDialogueTimespan( 0.0f )
    , reviveTutDialogueTimespan( 0.0f )
    , vineTutDialogueTimespan( 0.0f )
    , elevatorDialogueTimespan( 0.0f ) { }

TutorialDialogueManager::~TutorialDialogueManager(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto sm = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

    if (sm.size( ))
        sm[ 0 ]->GetComponent<LevelSegmentManager>( )->Listener( this )
            .Off( LevelSegmentManagerEvents::SegmentChanged, &TutorialDialogueManager::onSegmentChange );
}

void TutorialDialogueManager::OnSceneReady(Scene *scene)
{
    auto world = GetOwner( )->GetWorld( );

    auto sm = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

    if (sm.size( ) == 0)
        return;

    auto segManager = sm[ 0 ]->GetComponent<LevelSegmentManager>( );
    
    segManager->Listener( this )
        .On( LevelSegmentManagerEvents::SegmentChanged, &TutorialDialogueManager::onSegmentChange );

    dispatchUIEventForSegment( segManager->GetCurrentSegment( ) );
}

void TutorialDialogueManager::onSegmentChange(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    dispatchUIEventForSegment( sender->GetCurrentSegment( ) );
}

void TutorialDialogueManager::dispatchUIEventForSegment(LevelSegments segment)
{
    Array<std::string> *lines = nullptr;
    float *timespan = nullptr;

    switch (segment)
    {
        case LevelSegments::Tut_OpeningCinematic:
        {
            lines = &cinematicDialogueLines;
            timespan = &cinematicDialogueTimespan;
            break;
        }
        case LevelSegments::Tut_MovementTutorial:
        case LevelSegments::Tut_GateOpensTutorial:
        {
            lines = &movementTutDialogueLines;
            timespan = &movementTutDialogueTimespan;
            break;
        }
        case LevelSegments::Tut_JumpTutorial:
        {
            lines = &jumpTutDialogueLines;
            timespan = &jumpTutDialogueTimespan;
            break;
        }
        case LevelSegments::Tut_HipFireTutorial:
        case LevelSegments::Tut_AimFireTutorial:
        case LevelSegments::Tut_ShootMovingTargetsTutorial:
        {
            lines = &shootingTutDialogueLines;
            timespan = &shootingTutDialogueTimespan;
            break;
        }
        case LevelSegments::Tut_KillPlayerTutorial:
        {
            lines = &reviveTutDialogueLines;
            timespan = &reviveTutDialogueTimespan;
            break;
        }
        case LevelSegments::Tut_SpawnVinesTutorial:
        {
            lines = &vineTutDialogueLines;
            timespan = &elevatorDialogueTimespan;
            break;
        }
        case LevelSegments::BossRoom_Platforming:
        {
            lines = &elevatorDialogueLines;
            timespan = &elevatorDialogueTimespan;
            break;
        }
    }

    if (!lines || !timespan)
        return;

    auto *scene = GetOwner( )->GetWorld( )->GetOwner( );

    if (!scene)
        return;

    auto manager = scene->GetGameContext( )->GetManager( );

    if (!manager)
        return;

    auto *ui = manager->GetConfigComponent<UIScreensConfig>( );

    ui_event::TutorialDialogue event;

    event.dialogueLines = *lines;
    event.totalTimeForDialogue = *timespan;

    ui->TriggerPlayerHUDEvent( event );
}
