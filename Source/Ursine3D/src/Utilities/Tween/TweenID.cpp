#include "UrsinePrecompiled.h"

#include "TweenID.h"
#include "TweenManager.h"

#include "TweenDelay.h"
#include "TweenCall.h"

#include "TweenGrouped.h"

namespace ursine
{
    // invalid default constructor
    TweenID::TweenID(void)
        : m_id( -1 )
        , m_manager( nullptr ) { }

    bool TweenID::IsValid(void) const
    {
        return !!m_manager;
    }

    TweenID::TweenID(TweenManager *manager, uint32 id)
        : m_id( id )
        , m_manager( manager ) { }

    bool TweenID::IsActive(void) const
    {
        auto tween = m_manager->get( m_id );

        return tween && !tween->m_deleting;
    }

    void TweenID::Pause(void)
    {
        auto tween = m_manager->get( m_id );

        if (tween)
            tween->m_paused = true;
    }

    void TweenID::Resume(void)
    {
        auto tween = m_manager->get( m_id );

        if (tween)
            tween->m_paused = false;
    }

    void TweenID::Cancel(void) const
    {
        m_manager->cancel( m_id );
    }

    bool TweenID::IsPaused(void) const
    {
        auto tween = m_manager->get( m_id );

        if (!tween)
            return true;

        return tween->m_paused || m_manager->m_groups[ tween->m_group ];
    }

    TweenID &TweenID::Stop(void)
    {
        auto tween = m_manager->get( m_id );

        if (tween)
            tween->stop( );

        return *this;
    }

    TweenID &TweenID::Delay(const TimeSpan &duration)
    {
        auto tween = m_manager->get( m_id );

        if (tween)
            tween->add( new TweenDelay( duration ) );

        return *this;
    }

    TweenID &TweenID::Call(TweenCallback callback)
    {
        auto tween = m_manager->get( m_id );

        if (tween)
            tween->add( new TweenCall( callback ) );

        return *this;
    }

    TweenGrouped &TweenID::BeginGroup(void) const
    {
        auto tween = m_manager->get( m_id );

        UAssert( tween,
            "Cannot create group on invalid or deleted tween." );

        auto grouped = new TweenGrouped( m_manager, m_id );

        tween->add( grouped );

        return *grouped;
    }

    TweenItem *TweenID::GetCurrentItem(void) const
    {
        auto tween = m_manager->get( m_id );

        if (!tween)
            return nullptr;

        return tween->m_items.empty( ) ? nullptr : tween->m_items.front( );
    }

    TweenID &TweenID::Removed(TweenCallback callback)
    {
        auto tween = m_manager->get( m_id );

        if (tween)
            tween->m_removed = callback;

        return *this;
    }

    void TweenID::doProperty(const TimeSpan &duration, const TweenPercentageCallback &property_fn)
    {
        auto tween = m_manager->get( m_id );

        if (tween)
            tween->add( new TweenProperty( duration, property_fn ) );
    }
}
