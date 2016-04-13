/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialDialogueManagerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <Array.h>

#include "LevelSegmentManagerComponent.h"

class TutorialDialogueManager : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    TutorialDialogueManager(void);

    void OnSceneReady(ursine::Scene *scene) override;

    ursine::Array<std::string> cinematicDialogueLines;
    float cinematicDialogueTimespan;

    ursine::Array<std::string> movementTutDialogueLines;
    float movementTutDialogueTimespan;

    ursine::Array<std::string> jumpTutDialogueLines;
    float jumpTutDialogueTimespan;

    ursine::Array<std::string> shootingTutDialogueLines;
    float shootingTutDialogueTimespan;

    // @Austin @Josh these are commented out because AI isn't a thing right now and the
    // tutorial isn't made for it
    // ursine::Array<std::string> skirmishDialogueLines;
    // float skirmishDialogueTimespan;

    ursine::Array<std::string> reviveTutDialogueLines;
    float reviveTutDialogueTimespan;

    ursine::Array<std::string> vineTutDialogueLines;
    float vineTutDialogueTimespan;

    ursine::Array<std::string> elevatorDialogueLines;
    float elevatorDialogueTimespan;

private:
    void onSegmentChange(EVENT_HANDLER(LevelSegmentManager));

    void dispatchUIEventForSegment(LevelSegments segment);

} Meta( Enable );
