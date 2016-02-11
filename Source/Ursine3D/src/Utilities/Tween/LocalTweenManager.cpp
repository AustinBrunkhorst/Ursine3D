/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LocalTweenManager.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "LocalTweenManager.h"
#include "TweenManager.h"

namespace ursine
{
    LocalTweenManager::LocalTweenManager(TweenManager *manager, TweenGroupID defaultGroup)
        : m_defaultGroup( defaultGroup )
        , m_manager( manager ) { }

    LocalTweenManager::~LocalTweenManager(void)
    {
        ClearAll( );
    }

    TweenID LocalTweenManager::Create(void)
    {
        return Create( m_defaultGroup );
    }

    TweenID LocalTweenManager::Create(TweenGroupID group)
    {
        auto tween = Tween::Create( group );

        m_created[ tween.m_id ] = tween;

        return tween.Removed( [=] {
            m_created.erase( tween.m_id );
        } );
    }

    void LocalTweenManager::ClearAll(void)
    {
        for (auto &tween : m_created)
            m_manager->cancel( tween.second.m_id, false );

        m_created.clear( );
    }

    void LocalTweenManager::PauseAll(void)
    {
        for (auto &tween : m_created)
            tween.second.Pause( );
    }

    void LocalTweenManager::ResumeAll(void)
    {
        for (auto &tween : m_created)
            tween.second.Resume( );
    }
}
