/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace Ursine
{
    TweenManager *gTweenManager = nullptr;

    TweenManager::TweenManager(void)
    {
        gTweenManager = this;
    }

    TweenManager::~TweenManager(void)
    {
        gTweenManager = nullptr;
    }

    void TweenManager::Update(void)
    {
        for (auto &pair : _tweens)
        {
            auto &tween = pair.second;

            // deleting or paused?
            if (tween._deleting || tween._paused || _groups[tween._group])
                continue;

            tween.Update();
        }

        // delete those in the deletion queue
        for (auto &deleted : _deletion_queue)
            _tweens.erase(deleted);

        _deletion_queue.clear();
    }

    void TweenManager::Pause(TweenGroupID group)
    {
        if (group + 1u <= _groups.size())
            _groups[group] = true;
    }

    void TweenManager::Resume(TweenGroupID group)
    {
        if (group + 1u <= _groups.size())
            _groups[group] = false;
    }

    void TweenManager::Clear(TweenGroupID group)
    {
        for (auto it = _tweens.begin(); it != _tweens.end();)
        {
            if (it->second._group == group)
                _tweens.erase(it++);
            else
                ++it;
        }
    }

    TweenID TweenManager::create(TweenGroupID group)
    {
        auto id = _next_id++;

        if (group + 1u > _groups.size())
            _groups.resize(group + 1u, false);

        _tweens.emplace(std::make_pair(id, Tween(group)));

        return TweenID(id);
    }

    Tween *TweenManager::get(uint32 id)
    {
        auto search = _tweens.find(id);

        return search == _tweens.end() ? nullptr : &search->second;
    }

    void TweenManager::cancel(uint32 id, bool invoke_removed)
    {
        auto tween = get(id);

        if (tween && !tween->_deleting)
        {
            if (invoke_removed && tween->_removed)
                tween->_removed();

            tween->_deleting = true;

            _deletion_queue.push_back(id);
        }
    }
}