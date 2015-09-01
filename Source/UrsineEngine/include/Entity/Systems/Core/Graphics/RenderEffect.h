#pragma once

namespace ursine
{
    namespace ecs
    {
        class Renderable;

        class RenderEffect
        {
            const RenderEffect &operator=(const RenderEffect &rhs);

        public:
            virtual ~RenderEffect(void) { }

            virtual void Bind(World *world) = 0;
            virtual void Render(Renderable *renderable) = 0;
            virtual void UnBind(void) = 0;
        };
    }
}