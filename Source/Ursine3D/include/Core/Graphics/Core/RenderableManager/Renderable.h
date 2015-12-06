/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Renderable.h
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
File Name:      Renderable.h
Module:         Graphics
Purpose:        All of the possible renderables in graphics
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "EntityConfig.h"
#include "GFXAPIDefines.h"
#include <string>
#include "SMat4.h"
#include "SVec3.h"
#include "Vec2.h"
#include "Color.h"

namespace ursine
{
    namespace graphics
    {
        /////////////////////////////////////////////////////////////////
        //main renderable class
        class Renderable
        {
            friend class GfxManager;
            friend class RenderableManager;
        public:
            Renderable(void);
            void Initialize(void);
            void SetEntityUniqueID(const ecs::EntityUniqueID id);
            ecs::EntityUniqueID GetEntityUniqueID() const;
            void SetOverdraw(bool draw);
            bool GetOverdraw() const;

            void SetDebug(bool debug);
            bool GetDebug() const;

            RenderMask GetRenderMask(void) const;
            void SetRenderMask(const RenderMask mask);

        private:
            ecs::EntityUniqueID entityID;
            bool Active_;
            bool Overdraw_;
            bool Debug_;
            RenderMask m_mask;
        };

        /////////////////////////////////////////////////////////////////
        //main model class
        class Model : public Renderable
        {
            friend class RenderableManager;
        public:
            const SMat4 &GetWorldMatrix(void);
            void SetWorldMatrix(const SMat4 &matrix);

            Model(void);

        private:
            SMat4 Transform_;
        };

        /////////////////////////////////////////////////////////////////
        //3d model class
        class Model3D : public Model
        {
            friend class RenderableManager;
        public:
            Model3D(void);
            void Initialize(void);

            const char *GetModelName(void);
            void SetModel(std::string modelName);

            const char *GetMaterialslName(void);
            void SetMaterial(std::string materialName);

    
            void SetMaterialData(float emiss, float pow, float intensity);
            void GetMaterialData(float &emiss, float &pow, float &intensity);


            void SetAnimationTime(const float time);
            float &GetAnimationTime(void);

            void SetColor(const Color color);
            const Color &GetColor() const;

            std::vector<SMat4> &GetMatrixPalette(void);

        private:
            float m_emissive;
            float m_specPow;
            float m_specIntensity;
            Color m_color;
            std::string ModelName_;
            std::string MaterialName_;

            float m_animationTime;

            std::vector<SMat4> m_matrixPalette;
        };

        /////////////////////////////////////////////////////////////////
        //2d billboard class
        class Billboard2D : public Renderable
        {
            friend class RenderableManager;
        public:
            Billboard2D(void);

            const char *GetTextureName(void);

            void SetTexture(std::string texName);

            void SetDimensions(float width, float height);
            void GetDimensions(float &width, float &height);

            void SetPosition(const ursine::SVec3 &position);
            const ursine::SVec3 &GetPosition(void) const;
            
            void SetColor(const Color color);
            const Color &GetColor() const;

        private:
            float m_width;
            float m_height;
            ursine::Vec2 m_scale;
            ursine::SVec3 m_position;
            Color m_color;
            std::string TextureName_;
        };

        /////////////////////////////////////////////////////////////////
        //3d primitve class
        class Primitive : public Model
        {
        public:
            enum PRIMITIVE_TYPE
            {
                PRIM_SPHERE = 0,
                PRIM_CUBE, //done
                PRIM_PLANE,
                PRIM_CAPSULE, //done
                PRIM_COUNT
            };

            void Initialize(void);

            void SetType(PRIMITIVE_TYPE type);
            PRIMITIVE_TYPE GetType();

            void SetRadius(float r);
            float GetRadius();

            void SetWidth(float r);
            float GetWidth();

            void SetDepth(float r);
            float GetDepth();

            void SetHeight(float r);
            float GetHeight();

            void SetWireFrameMode(bool useWireframe);
            bool GetWireFrameMode();

            Color &GetColor();
            void SetColor(const Color &color);
            void SetColor(float r, float g, float b, float a = 1.0f);
        private:
            PRIMITIVE_TYPE Type_;
            float Radius_;
            float Height_;
            float Width_;
            float Depth_;
            bool UseWireFrame_;
            Color Color_;
        };

        ///////////////////////////////////////////////////////////////////
        // LIGHTS /////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////
        // universal light class
        class Light : public Renderable
        {
        public:
            //enum for the different types of lights
            enum LightType
            {
                LIGHT_DIRECTIONAL = 0,
                LIGHT_POINT,
                LIGHT_SPOTLIGHT,

                LIGHT_COUNT
            };

        public:
            void Initialize(void);

            LightType GetType(void);
            void SetType(const LightType type);

            const SVec3 &GetDirection(void);
            void SetDirection(const SVec3 &dir);
            void SetDirection(const float x, const float y, const float z);

            const SVec3 &GetPosition(void);
            void SetPosition(const SVec3 &position);
            void SetPosition(const float x, const  float y, const float z);

            const Color &GetColor(void);
            void SetColor(const Color &color);
            void SetColor(const float r, const float g, const float b);

            float GetRadius(void);
            void SetRadius(const float radius);

            float GetIntensity(void);
            void SetIntensity(const float intensity);

            const Vec2 &GetSpotlightAngles(void);
            void SetSpotlightAngles(const Vec2 &angles);
            void SetSpotlightAngles(const float inner, const float outer);

            void SetSpotlightTransform(const SMat4 &transf);
            const SMat4 &GetSpotlightTransform(void);

        private:
            LightType m_type;
            SVec3 m_position;
            Color m_color;
            float m_radius;
            SVec3 m_direction;
            float m_intensity;

            Vec2 m_spotlightAngles;
            SMat4 m_spotlightTransform;
        };
    }
}
