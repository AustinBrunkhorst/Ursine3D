/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorLightSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

// Draw the different light types
class LightDebugDrawSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    LightDebugDrawSystem(ursine::ecs::World *world);

private:
    void Process(const ursine::ecs::EntityHandle &entity) override;

} Meta(Enable, AutoAddEntitySystem);

// Draw the camera's frustum
class CameraDebugDrawSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    CameraDebugDrawSystem(ursine::ecs::World *world);

private:
    void Process(const ursine::ecs::EntityHandle &entity) override;

} Meta(Enable, AutoAddEntitySystem);

// Draw the audio emitter's range
class AudioEmitterDebugDrawSystem : public ursine::ecs::FilterSystem
{
    ENTITY_SYSTEM;

public:
    AudioEmitterDebugDrawSystem(ursine::ecs::World *world);

private:
    void Process(const ursine::ecs::EntityHandle &entity) override;

} Meta(Enable, AutoAddEntitySystem);
