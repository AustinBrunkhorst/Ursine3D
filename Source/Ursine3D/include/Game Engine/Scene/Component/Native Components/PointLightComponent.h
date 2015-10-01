#pragma once

#include "Component.h"
#include "Renderable.h"

namespace ursine
{
    namespace ecs
    {
        class PointLight : public Component
        {
            NATIVE_COMPONENT;

        public:
            PointLight( void );
            ~PointLight( void );

            void OnInitialize( void ) override;

            //get/set color
            void SetColor( const Color &color );
            void SetColor( const float &r, const float &g, const float &b );

            //get/set position
            void SetPosition( const SVec3 &position );
            void SetPosition( const float &x, const float &y, const float &z );

            //get/set radius
            void SetRadius( const float &radius );

        private:
            ursine::PointLight *m_pointLight;

            friend class RenderSystem;
        }Meta( Enable, DisplayName( "PointLight" ) );
    }
}