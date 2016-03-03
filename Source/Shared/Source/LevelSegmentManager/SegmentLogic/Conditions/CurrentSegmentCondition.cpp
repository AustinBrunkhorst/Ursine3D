/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CurrentSegmentCondition.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CurrentSegmentCondition.h"

namespace ursine
{
    namespace sm
    {
        CurrentSegmentCondition::CurrentSegmentCondition(
            LevelSegmentManager* manager, LevelSegments startSegment, LevelSegments endSegment
        )
            : Condition( "" )
            , m_manager( manager )
            , m_startSegment( startSegment )
            , m_endSegment( endSegment )
            , m_range( true )
        {
        }

        CurrentSegmentCondition::CurrentSegmentCondition(
            LevelSegmentManager* manager, LevelSegments startSegment
        )
            : Condition( "" )
            , m_manager( manager )
            , m_startSegment( startSegment )
            , m_range( true )
        {
        }

        bool CurrentSegmentCondition::GetCondition(StateMachine* stateMachine)
        {
            auto cur = m_manager->GetCurrentSegment( );

            if (m_range)
                return m_startSegment <= cur && cur <= m_endSegment;
            else
                return cur == m_startSegment;
        }
    }
}
