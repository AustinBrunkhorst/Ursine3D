#include "UrsinePrecompiled.h"

#include "RenderableComponent.h"

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

        GFXHND Renderable::GetHandle(void) const
        {
            return m_handle;
        }

        void Renderable::SetHandle(GFXHND handle)
        {
            m_handle = handle;
        }
    }
}