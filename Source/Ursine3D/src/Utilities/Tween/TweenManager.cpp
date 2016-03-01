/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TweenManager.h"

namespace ursine
{
    CORE_SYSTEM_DEFINITION( TweenManager );

    TweenManager::TweenManager(void)
        : m_nextID( 0 )
    {
        Application::Instance->Connect( APP_UPDATE, this, &TweenManager::onAppUpdate );
    }

    TweenManager::~TweenManager(void)
    {
        Application::Instance->Disconnect( APP_UPDATE, this, &TweenManager::onAppUpdate );
    }

    void TweenManager::Pause(TweenGroupID group)
    {
		std::lock_guard<std::mutex> lock( m_mutex );

        if (group < m_groups.size( ))
            m_groups[ group ] = true;
    }

    void TweenManager::Resume(TweenGroupID group)
    {
		std::lock_guard<std::mutex> lock( m_mutex );

        if (group < m_groups.size( ))
            m_groups[ group ] = false;
    }

    void TweenManager::Clear(TweenGroupID group)
    {
		std::lock_guard<std::mutex> lock( m_mutex );

        for (auto it = m_tweens.begin( ); it != m_tweens.end( );)
        {
            if (it->second.m_group == group)
                m_tweens.erase( it++ );
            else
                ++it;
        }
    }

    void TweenManager::onAppUpdate(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, EventArgs);

		std::lock_guard<std::mutex> lock( m_mutex );

        for (auto &pair : m_tweens)
        {
            auto &tween = pair.second;

            // deleting or paused?
            if (tween.m_deleting || tween.m_paused || m_groups[ tween.m_group ])
                continue;

            tween.Update( sender->GetDeltaTime( ) );
        }

        // delete those in the deletion queue
        for (auto &deleted : m_deletionQueue)
            m_tweens.erase( deleted );

        m_deletionQueue.clear( );
    }

    TweenID TweenManager::create(TweenGroupID group)
    {
        auto id = m_nextID++;

		std::lock_guard<std::mutex> lock( m_mutex );

        if (group >= m_groups.size( ))
            m_groups.resize( group + 1u, false );

        m_tweens.emplace( std::make_pair( id, Tween( group ) ) );

        return TweenID( this, id );
    }

    Tween *TweenManager::get(uint32 id)
    {
        auto search = m_tweens.find( id );

        return search == m_tweens.end( ) ? nullptr : &search->second;
    }

    void TweenManager::cancel(uint32 id, bool invoke_removed)
    {
		std::lock_guard<std::mutex> lock( m_mutex );

        auto tween = get( id );

        if (tween && !tween->m_deleting)
        {
            if (invoke_removed && tween->m_removed)
                tween->m_removed( );

            tween->m_deleting = true;

            m_deletionQueue.push_back( id );
        }
    }
}
