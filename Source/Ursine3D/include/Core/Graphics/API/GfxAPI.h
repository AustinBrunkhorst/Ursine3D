/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxAPI.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GfxAPI.h
Module:         Graphics
Purpose:        Interface for the bulk of graphics
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include "GfxConfig.h"
#include "GfxAPIDefines.h"
#include "ResourceAPI.h"
#include "RenderableAPI.h"
#include "CameraAPI.h"
#include "ViewportAPI.h"
#include "UIAPI.h"
#include "DrawingAPI.h"
#include "EntityConfig.h"
#include "CoreSystem.h"

namespace ursine
{
    namespace graphics
    {
        class GfxAPI : public core::CoreSystem
        {
            CORE_SYSTEM
        private:
            struct privateData;

            //public members
        public:
            //manage resources like textures, models
            //RenderableManager
            Meta(Disable)
            ResourceAPI ResourceMgr;

            Meta(Disable)
            RenderableAPI RenderableMgr;

            Meta(Disable)
            CameraAPI CameraMgr;

            Meta(Disable)
            ViewportAPI ViewportMgr;

            Meta(Disable)
            UIAPI UIMgr;

            Meta(Disable)
            DrawingAPI DrawingMgr;

            //public methods
        public:
            Meta(Enable, DisableNonDynamic)
            GfxAPI(void);
            ~GfxAPI(void);

            void OnInitialize(void) override;
             void OnRemove(void) override;

            // initialize graphics manager
            void StartGraphics(GfxConfig config);

            // begin graphics for the whole frame
            void StartFrame(void);

            // render an object                   
            void RenderObject(GfxHND handle);

            // begin to render a scene   
            void BeginScene(void);

            // render the scene
             void RenderScene(float dt, GfxHND camera);

            // end the current scene
            void EndScene(void);

            // end the whole frame
            void EndFrame(void);

            // resize the window
            void Resize(int width, int height);

            // set fullscreen state
            void SetFullscreenState(const bool state);

            // set the main screen viewport. All cameras are sized 
            // w/ respect to this size. Should always be width
            // and height of the window
            void SetGameViewport(GfxHND vp);

            // render a texture to the main viewport
            void RenderTextureMain(GfxHND &handle, const float posX, const float posY);

            // render a texture in a specific viewport
            void RenderTextureInViewport(GfxHND &handle, const float posX, const float posY, GfxHND &camera);

            // render a buffer of data to a dynamic texture
            void RenderToDynamicTexture(const int srcWidth, const int srcHeight, 
                                        const void *input, 
                                        const int inputWidth, const int inputHeight, 
                                        GfxHND destTexture, 
                                        const int destinationX, const int destinationY);

            // get current object that is moused over
            ursine::ecs::EntityUniqueID GetMousedOverID(void);

            SVec3 GetMousedOverWorldPosition( const GfxHND &cameraHandle );
            
            // private members
        private:
            privateData *m_privates;

        } Meta(Enable, WhiteListMethods);
    }
}
