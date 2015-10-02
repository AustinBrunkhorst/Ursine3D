#include "UrsinePrecompiled.h"

#include "RenderableComponent.h"
#include "Renderable.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Renderable );

        Renderable::Renderable(void)
            : BaseComponent( )
            , m_handle( 0 )
        {
            
        }

        graphics::GfxHND Renderable::GetHandle(void) const
        {
            return m_handle;
        }

        void Renderable::SetHandle(graphics::GfxHND handle)
        {
            m_handle = handle;
        }
    }
}