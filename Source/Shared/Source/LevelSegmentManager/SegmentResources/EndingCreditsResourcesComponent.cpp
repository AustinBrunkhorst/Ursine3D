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
            break;
        }
    }

    if (!duration)
        return;

    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto settings = world->GetSettings( );

    if (!settings)
        return;

    auto ui = settings->GetComponent<UIScreensConfig>( );

    if (!ui)
        return;

    ui_event::TutorialDialogue event;

    event.dialogueLines = *text;
    event.totalTimeForDialogue = *duration;

    ui->TriggerPlayerHUDEvent( event );
}
