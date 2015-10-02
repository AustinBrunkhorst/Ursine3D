 #pragma once

#include "Component.h"
#include "Renderable.h"

namespace ursine
{
    namespace ecs
    {
        class DirectionalLight : public Component
        {
            NATIVE_COMPONENT;

        public:
            DirectionalLight( void );
            ~DirectionalLight( void );

            void OnInitialize( void ) override;
            
            //get/set direction
            const SVec3 &GetDirection( void ) const;
            void SetDirection( const SVec3 &dir );
            void SetDirection( const float &x, const float &y, const float &z );

            //get/set color
            const Color &GetColor( void ) const;
            void SetColor( const Color &color );
            void SetColor( const float r, const float g, const float b );
            
        private:
            ursine::DirectionalLight *m_directionalLight;

            friend class RenderSystem;
        }Meta( Enable, DisplayName( "DirectionalLight" ) );
    }
}