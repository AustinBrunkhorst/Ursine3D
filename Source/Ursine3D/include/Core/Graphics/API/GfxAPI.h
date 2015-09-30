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
#include "ResourceAPI.h"
#include "RenderableAPI.h"
#include "CameraAPI.h"
#include "ViewportAPI.h"
#include "UIAPI.h"
#include "DrawingAPI.h"

#include "CoreSystem.h"

#include "Meta.h"

namespace ursine
{
    class GfxAPI : public ursine::core::CoreSystem
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
        Meta( DisableNonDynamic ) 
        GfxAPI(void);

        void OnInitialize(void) override;

        void OnRemove(void) override;

        // initialize graphics manager
        void StartGraphics(GfxConfig config);

        // begin graphics for the whole frame
        void StartFrame();

        // render an object                                                                   
        void RenderObject(GFXHND handle);

        // begin to render a scene                                                                   
        void BeginScene();

        // render the scene
        void RenderScene(float dt, GFXHND camera);

        // end the current scene
        void EndScene();

        // end the whole frame
        void EndFrame();

        // resize the window
        void Resize(int width, int height);

        //set the main game viewport. All cameras are sized 
            // w/ respect to this size
        void SetGameViewport(GFXHND vp);

        // private members
    private:
        privateData *m_privates;
    } Meta(Enable);
}
