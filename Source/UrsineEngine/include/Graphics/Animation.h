/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Animation.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ContentManager.h"
#include "TimerID.h"

#include "AnimationEvent.h"

#include "Vec2.h"

#include "JsonSerializer.h"

namespace Ursine
{
    class Texture2D;

    class Animation 
        : public Asset
        , public EventDispatcher<AnimationEvent>
    {
        friend class ContentManager;

        // texture atlas for this animation
        Texture2D *_texture;

        // current frame index
        uint32 _frame_index;

        int _frame_direction;

        // total number of frames in the animation
        uint32 _frame_count;

        // number of frames in a single line of the texture atlas
        uint32 _frames_per_line;

        // size of each frame
        Vec2 _frame_size;

        // duration elapsed in the current frame (proportional to frame_duration)
        float _frame_elapsed;

        // duration of each frame (in seconds)
        float _frame_duration;

        // used to normalize frame size and offset to UV coordinates
        Vec2 _uv_scale;

        // offset in uv coordinates
        Vec2 _uv_offset;

        // determines if the animation loops
        bool _loop;

        // if looping, determines if the animation runs in a ping pong fashion
        bool _ping_pong;

        // deteremines if the animation is currently updating
        bool _paused;

        URSINE_TODO("Remove this");
        bool _frame_changed;

    public:
        Animation(const std::string &path);
        Animation(const Animation &rhs);

        ~Animation(void);

        void Update(void);

        // Resumes the animation
        void Play(void);

        // Pauses the animation
        void Pause(void);

        Texture2D *GetTexture(void);

        const Vec2 &GetUVOffset(void);
        const Vec2 &GetUVScale(void);

        URSINE_TODO("Remove this");
        bool FrameChanged(void) const;
    };

    extern template
    Animation *ContentManager::Loader(const std::string &name, uint32 flags);

    extern template
    Animation *ContentManager::Copier(const Animation *instance, uint32 flags);

    extern template
    Json JsonSerializer::Serialize(Animation* &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, Animation* &out);
}