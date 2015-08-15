#include "UrsinePrecompiled.h"

#include "LocalTweenManager.h"
#include "TweenManager.h"

namespace Ursine
{
    LocalTweenManager::LocalTweenManager(TweenGroupID default_group)
        : _default_group(default_group)
    {

    }

    LocalTweenManager::~LocalTweenManager(void)
    {
        ClearAll();
    }

    TweenID LocalTweenManager::Create(void)
    {
        return Create(_default_group);
    }

    TweenID LocalTweenManager::Create(TweenGroupID group)
    {
        auto tween = Tween::Create(group);

        _created[tween._id] = tween;

        return tween.Removed([=] {
            _created.erase(tween._id);
        });
    }

    void LocalTweenManager::ClearAll(void)
    {
        for (auto &tween : _created)
            gTweenManager->cancel(tween.second._id, false);

        _created.clear();
    }

    void LocalTweenManager::PauseAll(void)
    {
        for (auto &tween : _created)
            tween.second.Pause();
    }

    void LocalTweenManager::ResumeAll(void)
    {
        for (auto &tween : _created)
            tween.second.Resume();
    }
}