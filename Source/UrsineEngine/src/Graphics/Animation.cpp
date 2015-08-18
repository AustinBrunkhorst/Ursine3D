/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Animation.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "Animation.h"
#include "Texture2D.h"

#include "Json.h"

namespace Ursine
{
    Animation::Animation(const std::string &path)
        : Asset(path)
        , EventDispatcher(this)
        , _frame_index(0)
        , _frame_direction(1)
        , _paused(false)
    {
        
    }

    Animation::Animation(const Animation &rhs)
        : Asset(rhs._path)
        , _texture(rhs._texture)
        , _frame_index(0)
        , _frame_direction(1)
        , _frame_count(rhs._frame_count)
        , _frames_per_line(rhs._frames_per_line)
        , _frame_size(rhs._frame_size)
        , _frame_elapsed(0.0f)
        , _frame_duration(rhs._frame_duration)
        , _uv_scale(rhs._uv_scale)
        , _uv_offset({ 0.0f, 0.0f }) // TODO
        , _loop(rhs._loop)
        , _ping_pong(rhs._ping_pong)
        , _paused(false)
    {
        
    }

    Animation::~Animation(void)
    {
        
    }

    void Animation::Update(void)
    {
        _frame_changed = false;

        if (_paused || _frame_count < 2)
            return;

        float dt = gApplication->GetDelta();

        auto last_index = _frame_index;

        _frame_elapsed += dt;

        // waiting on next frame
        if (_frame_elapsed < _frame_duration)
            return;

        _frame_elapsed = 0.0f;

        if (_loop)
        {
            if (_ping_pong)
            {
                int index = _frame_index + _frame_direction;

                // negate frame direction if we've reached the beginning or end
                // of the animation
                if (index < 0 || static_cast<uint32>(index) >= _frame_count)
                    _frame_direction = -_frame_direction;

                _frame_index += _frame_direction;
            }
            else
            {
                _frame_index = (_frame_index + 1u) % _frame_count;
            }
        }
        else if (_frame_index + 1u < _frame_count)
        {
            ++_frame_index;
        }

        // frame changed
        if (_frame_index != last_index)
        {
            _uv_offset.X() = (_frame_index % _frames_per_line) * _uv_scale.X();
            _uv_offset.Y() = (_frame_index / _frames_per_line) * _uv_scale.Y();

            _frame_changed = true;

            Dispatch(AE_FRAME_CHANGED, EventArgs::Empty);
        }
    }

    void Animation::Play(void)
    {
        _paused = false;
    }

    void Animation::Pause(void)
    {
        _paused = true;
    }

    Texture2D* Animation::GetTexture(void)
    {
        return _texture;
    }

    const Vec2 &Animation::GetUVOffset(void)
    {
        return _uv_offset;
    }

    const Vec2 &Animation::GetUVScale(void)
    {
        return _uv_scale;
    }

    bool Animation::FrameChanged(void) const
    {
        return _frame_changed;
    }

    template<>
    Animation *ContentManager::Loader(const std::string &name, uint32 flags)
    {
        auto text = LoadText(name);

        std::string error;

        auto json = Json::parse(*text, error);

        UAssert(error.empty(), 
            "Error parsing Animation JSON (%s)\n%s",
            name.c_str(), error.c_str());

        UAssert(json.is_object(), 
            "Expecting Animation root to be object.\n%s", name.c_str());

        auto animation = new Animation(name);

        auto &texture = json["texture"];

        UAssert(!texture.is_null(), 
            "Animation missing texture property.\n%s", name.c_str());

        JsonSerializer::Deserialize(texture, animation->_texture);

        auto &frame_width = json["frame_width"];

        UAssert(!frame_width.is_null(),
            "Animation missing frame width property.\n%s", name.c_str());

        JsonSerializer::Deserialize(frame_width, animation->_frame_size.X());

        auto &frame_count = json["frame_count"];

        UAssert(!frame_count.is_null(),
            "Animation missing frame count property.\n%s", name.c_str());

        JsonSerializer::Deserialize(frame_count, animation->_frame_count);

        auto &frame_duration = json["frame_duration"];

        UAssert(!frame_duration.is_null(),
            "Animation missing frame duration property.\n%s", name.c_str());

        JsonSerializer::Deserialize(frame_duration, animation->_frame_duration);

        JsonSerializer::Deserialize(json["loop"], animation->_loop, true);
        JsonSerializer::Deserialize(json["ping_pong"], animation->_ping_pong, false);

        animation->_frames_per_line = 
            static_cast<uint32>(animation->_texture->m_size.X() / animation->_frame_size.X());

        animation->_frame_size.Y() = animation->_texture->m_size.Y() / 
            (static_cast<float>(animation->_frame_count) / animation->_frames_per_line);

        animation->_uv_scale.X() =
            animation->_frame_size.X() / animation->_texture->m_size.X();

        animation->_uv_scale.Y() = 
            animation->_frame_size.Y() / animation->_texture->m_size.Y();

        return animation;
    }

    template<>
    Animation *ContentManager::Copier(const Animation *instance, uint32 flags)
    {
        return new Animation(*instance);
    }

    template<>
    Json JsonSerializer::Serialize(Animation* &instance)
    {
        return Json(instance->GetPath());
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, Animation* &out)
    {
        out = gContentManager->Load<Animation>(data.string_value(), ContentManager::COPY);
    }
}