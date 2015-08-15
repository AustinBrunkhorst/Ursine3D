/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderableComponent.hpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
**
** -------------------------------------------------------------------------*/

#include "RenderSystem.h"

namespace Ursine
{
    namespace ECS
    {
        RenderLayer Renderable::GetLayer(void)
        {
            return layer;
        }

        Texture2D *Renderable::GetTexture(void)
        {
            return texture;
        }
    }
}