/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderSystem.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#include "EntitySystem.h"

#include "EntityManager.h"
#include "SystemManager.h"

#include "RenderLayer.h"
#include "RenderEffects.h"

#include <map>
#include <array>

namespace Ursine
{
    namespace ECS
    {
        class Renderable;

        class RenderSystem : public EntitySystem
        {
            void onEntityAdded(EVENT_HANDLER(EntityManager));
            void onEntityRemoved(EVENT_HANDLER(EntityManager));

            void onDraw(EVENT_HANDLER(SystemManager));

            static bool compareTexture(const Entity *a, const Entity *b);

            // layers need to be iterated over in sorted order
            // we can assume that there will be a small amount of layers
            std::map<RenderLayer, 
                std::array<
                    std::deque<Entity*>,
                    FX_COUNT
                >
            > _renderable;

        public:
            RenderSystem(World *world);
            ~RenderSystem(void);

            void OnInitialize(void) override;
            void OnRemove(void) override;

            // Called when a renderable component's render layer changes
            void OnLayerChanged(Entity *a, RenderLayer layer_old, RenderLayer layer_new);

            // Called when a renderable component's texture changes
            void OnTextureChanged(Entity *a, Renderable *component);
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
#endif // !RENDER_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_SYSTEMS

#ifndef RENDER_SYSTEM_REGISTERED
#define RENDER_SYSTEM_REGISTERED

namespace Ursine 
{ 
    namespace ECS 
    {
        RegisterEntitySystem(RenderSystem);
    } 
}

#endif

#endif
