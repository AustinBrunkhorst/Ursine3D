/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EndingCreditsResourcesComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EndingCreditsResourcesComponent.h"

#include "UIEvents.h"
#include "UIScreensConfigComponent.h"

#include <Scene.h>

NATIVE_COMPONENT_DEFINITION( EndingCreditsResources );

using namespace ursine;
using namespace ecs;

EndingCreditsResources::EndingCreditsResources(void)
    : BaseComponent( ) { }

EndingCreditsResources::~EndingCreditsResources(void)
{
    auto sm = GetOwner( )->GetComponent<LevelSegmentManager>( );

    if (!sm)
        return;

    sm->Listener( this )
        .Off( LevelSegmentManagerEvents::SegmentChanged, &EndingCreditsResources::onSegmentChange );
}

void EndingCreditsResources::OnSceneReady(Scene *scene)
{
    auto sm = GetOwner( )->GetComponent<LevelSegmentManager>( );

    sm->Listener( this )
        .On( LevelSegmentManagerEvents::SegmentChanged, &EndingCreditsResources::onSegmentChange );
}

const ursine::resources::ResourceReference &EndingCreditsResources::GetWorldData(void) const
{
    return m_worldToMerge;
}

void EndingCreditsResources::SetWorldData(const ursine::resources::ResourceReference &world)
{
    m_worldToMerge = world;

    if (!resourcesAreAvailable( ))
        return;

    NOTIFY_COMPONENT_CHANGED( "world", m_worldToMerge );
}

void EndingCreditsResources::onSegmentChange(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    ursine::Array<std::string> *text = nullptr;
    float *duration = nullptr;
    bool endingCredits = false;

    switch (args->segment)
    {
        case LevelSegments::BossRoom_Phase5:
        {
            text = &phase5Text;
            duration = &phase5TextDuration;
            break;
        }
        case LevelSegments::EndingCredits:
        {
            text = &creditsText;
            duration = &creditsTextDuration;

            // Create a timer telling the UI to spawn the ending credits
            endingCredits = true;

            break;
        }
    }

    if (!duration)
        return;

    auto *scene = GetOwner( )->GetWorld( )->GetOwner( );

    if (!scene)
        return;

    auto manager = scene->GetGameContext( )->GetManager( );

    if (!manager)
        return;

    auto *ui = manager->GetConfigComponent<UIScreensConfig>( );

    ui_event::TutorialDialogue event;

    event.dialogueLines = *text;
    event.totalTimeForDialogue = *duration;

    ui->TriggerPlayerHUDEvent( event );

    if (endingCredits)
    {
        GetOwner( )->GetTimers( ).Create( TimeSpan::FromSeconds( *duration ) )
            .Completed( [=] {
                ui_event::SpawnEndingCredits creditsEvent;

                ui->TriggerPlayerHUDEvent( creditsEvent );
            } );
    }
}
