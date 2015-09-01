#include "UrsinePrecompiled.h"

#include "TweenGrouped.h"

#include "TweenDelay.h"
#include "TweenCall.h"

namespace ursine
{
    TweenGrouped::TweenGrouped(uint32 owner)
        : _owner(owner)
    {

    }

    TweenGrouped::~TweenGrouped(void)
    {
        for (auto *item : _items)
            delete item;
    }

    bool TweenGrouped::Update(void)
    {
        auto it = _items.begin();

        while (it != _items.end())
        {
            auto item = *it;

            if (item->Update())
            {
                delete item;

                it = _items.erase(it);
            }
            else
            {
                ++it;
            }
        }

        return _items.empty();
    }

    TweenGrouped &TweenGrouped::Delay(const TimeSpan &duration)
    {
        _items.push_back(new TweenDelay(duration));

        return *this;
    }

    TweenGrouped &TweenGrouped::Call(TweenCallback callback)
    {
        _items.push_back(new TweenCall(callback));

        return *this;
    }

    TweenID TweenGrouped::EndGroup(void) const
    {
        return TweenID(_owner);
    }

    void TweenGrouped::doProperty(const TimeSpan &duration, 
        const TweenPercentageCallback &property_fn)
    {
        _items.push_back(new TweenProperty(duration, property_fn));
    }
}