/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CurrentSegmentCondition.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Condition.h"
#include "LevelSegmentManagerComponent.h"
#include "LevelSegments.h"

namespace ursine
{
    namespace sm
    {
        class CurrentSegmentCondition : public Condition
        {
        public:
            CurrentSegmentCondition(
                LevelSegmentManager *manager, LevelSegments startSegment, LevelSegments endSegment
            );

            CurrentSegmentCondition(
                LevelSegmentManager *manager, LevelSegments segment
            );

            bool GetCondition(StateMachine *stateMachine) override;

        private:
            LevelSegmentManager *m_manager;
            LevelSegments m_startSegment;
            LevelSegments m_endSegment;
            bool m_range;
        };
    }
}
