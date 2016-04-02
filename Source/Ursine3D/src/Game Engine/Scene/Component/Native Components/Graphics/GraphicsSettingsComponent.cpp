/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GraphicsSettingsComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GraphicsSettingsComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(GraphicsSettings);

        GraphicsSettings::GraphicsSettings(void)
            : BaseComponent()
        {
        }

        GraphicsSettings::~GraphicsSettings(void)
        {
        }

        void GraphicsSettings::OnInitialize(void)
        {
        }

        float GraphicsSettings::GetGlobalEmissive(void) const
        {
            return GetCoreSystem(graphics::GfxAPI)->GetGlobalEmissive( );
        }

        void GraphicsSettings::SetGlobalEmissive(float value)
        {
            GetCoreSystem(graphics::GfxAPI)->SetGlobalEmissive( value );
        }

        float GraphicsSettings::GetLightStepValue(void) const
        {
            return GetCoreSystem(graphics::GfxAPI)->GetLightStepValue( );
        }

        void GraphicsSettings::SetLightStepValue(float value)
        {
            GetCoreSystem(graphics::GfxAPI)->SetLightStepValue( value );
        }
    }
}