#include "UrsinePrecompiled.h"

#include "TweenID.h"
#include "TweenManager.h"

#include "TweenDelay.h"
#include "TweenCall.h"

#include "TweenGrouped.h"

namespace Ursine
{
    // overflow will make this large, and thus invalid (will never conflict
    // with a real ID)
    TweenID::TweenID(void)
        : _id(-1) {}

    TweenID::TweenID(uint32 id)
        : _id(id) {}

    bool TweenID::IsActive(void)
    {
        auto tween = gTweenManager->get(_id);

        return tween && !tween->_deleting;
    }

    void TweenID::Pause(void)
    {
        auto tween = gTweenManager->get(_id);

        if (tween)
            tween->_paused = true;
    }

    void TweenID::Resume(void)
    {
        auto tween = gTweenManager->get(_id);

        if (tween)
            tween->_paused = false;
    }

    void TweenID::Cancel(void) const
    {
        gTweenManager->cancel(_id);
    }

    bool TweenID::IsPaused(void) const
    {
        auto tween = gTweenManager->get(_id);

        if (!tween)
            return true;

        return tween->_paused || gTweenManager->_groups[tween->_group];
    }

    TweenID &TweenID::Stop(void)
    {
        auto tween = gTweenManager->get(_id);

        if (tween)
            tween->stop();

        return *this;
    }

    TweenID &TweenID::Delay(const TimeSpan &duration)
    {
        auto tween = gTweenManager->get(_id);

        if (tween)
            tween->add(new TweenDelay(duration));

        return *this;
    }

    TweenID &TweenID::Call(TweenCallback callback)
    {
        auto tween = gTweenManager->get(_id);

        if (tween)
            tween->add(new TweenCall(callback));

        return *this;
    }

    TweenGrouped &TweenID::BeginGroup(void) const
    {
        auto tween = gTweenManager->get(_id);

        UAssert(tween, "Cannot create group on invalid or deleted tween.");

        auto grouped = new TweenGrouped(_id);

        tween->add(grouped);

        return *grouped;
    }

    TweenItem *TweenID::GetCurrentItem(void) const
    {
        auto tween = gTweenManager->get(_id);

        if (!tween)
            return nullptr;

        return tween->_items.empty( ) ? nullptr : tween->_items.front( );
    }

    TweenID &TweenID::Removed(TweenCallback callback)
    {
        auto tween = gTweenManager->get(_id);

        if (tween)
            tween->_removed = callback;

        return *this;
    }

    void TweenID::doProperty(const TimeSpan &duration, const TweenPercentageCallback &property_fn)
    {
        auto tween = gTweenManager->get(_id);

        if (tween)
            tween->add(new TweenProperty(duration, property_fn));
    }
}