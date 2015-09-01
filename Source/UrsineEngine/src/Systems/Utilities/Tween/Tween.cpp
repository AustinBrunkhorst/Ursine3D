/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Action.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Tween.h"
#include "TweenManager.h"

namespace ursine
{
    Tween::Tween(TweenGroupID group)
        : _group(group)
        , _deleting(false)
        , _paused(false)
        , _removed(nullptr) {}

    Tween::~Tween(void)
    {
        for (auto *item : _items)
            delete item;
    }

    void Tween::Update(void)
    {
        if (_items.empty())
            return;

        auto current = _items.front();

        if (current->Update())
        {
            delete current;

            _items.pop_front();
        }
    }

    TweenID Tween::Create(TweenGroupID group)
    {
        return gTweenManager->create(group);
    }

    void Tween::add(TweenItem *item)
    {
        _items.push_back(item);
    }

    void Tween::stop(void)
    {
        for (auto *item : _items)
            delete item;

        _items.clear();
    }
}