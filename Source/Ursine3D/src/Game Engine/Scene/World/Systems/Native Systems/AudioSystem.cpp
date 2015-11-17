#include "UrsinePrecompiled.h"

#include "AudioSystem.h"

namespace ursine
{
	namespace ecs
	{
		ENTITY_SYSTEM_DEFINITION(AudioSystem);

		AudioSystem::AudioSystem(World *world)
			: EntitySystem(world)
		{
			m_audioMan = GetCoreSystem(AudioManager);
		}

		void AudioSystem::OnInitialize(void)
		{
			m_world->Listener(this)
				.On(WORLD_RENDER, &AudioSystem::onUpdate)
				.On(WORLD_ENTITY_COMPONENT_ADDED, &AudioSystem::onComponentAdded)
				.On(WORLD_ENTITY_COMPONENT_REMOVED, &AudioSystem::onComponentRemoved);
		}

		void AudioSystem::OnRemove(void)
		{
			m_world->Listener(this)
				.Off(WORLD_RENDER, &AudioSystem::onUpdate)
				.Off(WORLD_ENTITY_COMPONENT_ADDED, &AudioSystem::onComponentAdded)
				.Off(WORLD_ENTITY_COMPONENT_REMOVED, &AudioSystem::onComponentRemoved);
		}

		void AudioSystem::onComponentAdded(EVENT_HANDLER(World))
		{
			EVENT_ATTRS(World, ComponentEventArgs);

			if (args->component->Is<AudioEmitter>())
			{
				m_emitters.emplace(
					args->entity->GetUniqueID(),
					static_cast<AudioEmitter*>(const_cast<Component*>(args->component))
					);
			}
		}

		void AudioSystem::onComponentRemoved(EVENT_HANDLER(World))
		{
			EVENT_ATTRS(World, ComponentEventArgs);

			if (args->component->Is<AudioEmitter>())
			{
				auto search = m_emitters.find(args->entity->GetUniqueID());

				if (search != m_emitters.end())
					m_emitters.erase(search);
			}
		}

		void AudioSystem::onUpdate(EVENT_HANDLER(World))
		{
			// walk through all emitters
			// update positions
			// Post Events
			for (auto &emitter : m_emitters)
			{
				if (emitter.second)
					renderable.second->updateRenderer();

				m_graphics->RenderObject(renderable.second->m_handle);
			}

			for (auto &camera : m_cameras)
			{
				e.camera = camera.second->m_handle;

				Dispatch(RENDER_HOOK, &e);

				m_graphics->RenderScene(0.0f, camera.second->m_handle);
			}

			m_graphics->EndScene();
		}
	}
}