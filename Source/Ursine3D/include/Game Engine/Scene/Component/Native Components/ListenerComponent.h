#pragma once

#include "Component.h"

#include "WwiseForward.h"

namespace ursine
{
	namespace ecs
	{
		class Listener : public Component
		{
			NATIVE_COMPONENT;

		public:
			Listener(void);
			~Listener(void);

			Meta(Disable)
				Listener GetHandle(void) const; //?

			Meta(Disable)
				void SetHandle(Listener handle); //?

			void OnInitialize(void) override;

		private:
			friend class RenderSystem;

			AkGameObjectID m_handle;

			// lets us know if we need to update the listener position
			bool m_dirty;

			void onTransformChange(EVENT_HANDLER(Entity));

			// notify the components listening to the 
			// renderable component to update the renderer
			// Ex: Model updates the renderers matrix
			void updateRenderer(void);

		} Meta(Enable, DisplayName("Renderable"));
	}
}