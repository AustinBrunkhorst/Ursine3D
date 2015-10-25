#include "UrsinePrecompiled.h"
#include "Renderable.h"

namespace ursine
{
    namespace graphics
    {
        ///////////////////////////////////////////////////////////////////
        // renderable class
        Renderable::Renderable()
        {
            Active_ = false;
        }

        void Renderable::Initialize() {}

        void Renderable::SetEntityUniqueID(const ecs::EntityUniqueID id)
        {
            entityID = id;
        }

        ecs::EntityUniqueID Renderable::GetEntityUniqueID() const
        {
            return entityID;
        }

        ///////////////////////////////////////////////////////////////////
        //model class
        Model::Model(void)
        {
            Transform_ = SMat4::Identity();
        }

        const SMat4 &Model::GetWorldMatrix(void)
        {
            return Transform_;
        }

        void Model::SetWorldMatrix(const SMat4 &matrix)
        {
            Transform_ = matrix;
        }

        ///////////////////////////////////////////////////////////////////
        //model3d
        void Model3D::Initialize()
        {
            ModelName_ = "Cube";
            MaterialName_ = "UV";

            m_emissive = 0;
            m_specPow = 0;
            m_specIntensity = 0;
        }

        void Model3D::SetMaterialData(float emiss, float pow, float intensity)
        {
            m_emissive = emiss;
            m_specPow = pow;
            m_specIntensity = intensity;
        }

        void Model3D::GetMaterialData(float &emiss, float &pow, float &intensity)
        {
            emiss = m_emissive;
            pow = m_specPow;
            intensity = m_specIntensity;
        }

        const char *Model3D::GetModelName(void)
        {
            return ModelName_.c_str();
        }

        void Model3D::SetModel(std::string modelName)
        {
            ModelName_ = modelName;
        }

        const char *Model3D::GetMaterialslName(void)
        {
            return MaterialName_.c_str();
        }

        void Model3D::SetMaterial(std::string materialName)
        {
            MaterialName_ = materialName;
        }


        ///////////////////////////////////////////////////////////////////
        //billboard2d
        Billboard2D::Billboard2D(void)
        {
            TextureName_ = "Default";
            m_width = 1;
            m_height = 1;
        }

        const char *Billboard2D::GetTextureName(void)
        {
            return TextureName_.c_str();
        }

        void Billboard2D::SetTexture(std::string texName)
        {
            TextureName_ = texName;
        }

        void Billboard2D::SetDimensions(float width, float height)
        {
            m_width = width;
            m_height = height;
        }

        void Billboard2D::GetDimensions(float &width, float &height)
        {
            width = m_width;
            height = m_height;
        }

        ///////////////////////////////////////////////////////////////////
        //primitives
        void Primitive::Initialize()
        {
            Type_ = Primitive::PRIM_CUBE;
            Radius_ = 1;
            Height_ = 1;
            Width_ = 1;
            Depth_ = 1;
            UseWireFrame_ = true;
            Color_ = Color(1.0f, 1.0f, 1.0f, 1.0f);
        }

        void Primitive::SetType(PRIMITIVE_TYPE type)
        {
            Type_ = type;
        }

        Primitive::PRIMITIVE_TYPE Primitive::GetType()
        {
            return Type_;
        }

        void Primitive::SetRadius(float r)
        {
            Radius_ = r;
        }

        float Primitive::GetRadius()
        {
            return Radius_;
        }

        void Primitive::SetWidth(float r)
        {
            Width_ = r;
        }

        float Primitive::GetWidth()
        {
            return Width_;
        }

        void Primitive::SetDepth(float r)
        {
            Depth_ = r;
        }

        float Primitive::GetDepth()
        {
            return Depth_;
        }

        void Primitive::SetHeight(float r)
        {
            Height_ = r;
        }

        float Primitive::GetHeight()
        {
            return Height_;
        }

        void Primitive::SetWireFrameMode(bool useWireframe)
        {
            UseWireFrame_ = useWireframe;
        }

        bool Primitive::GetWireFrameMode()
        {
            return UseWireFrame_;
        }

        Color &Primitive::GetColor()
        {
            return Color_;
        }

        void Primitive::SetColor(const Color &color)
        {
            Color_ = color;
        }

        void Primitive::SetColor(float x, float y, float z, float a)
        {
            Color_ = Color(x, y, z, a);
        }

        ///////////////////////////////////////////////////////////////////
        //lights

        ///////////////////////////////////////////////////////////////////
        // directional light
        DirectionalLight::DirectionalLight()
        {
            Direction_ = Vec3(0, 0, 1);
            Color_ = Color(1, 1, 1, 1);
        }

        SVec3 &DirectionalLight::GetDirection()
        {
            return Direction_;
        }

        void DirectionalLight::SetDirection(const SVec3 &dir)
        {
            Direction_ = dir;
            Direction_.Normalize();
        }

        void DirectionalLight::SetDirection(float x, float y, float z)
        {
            Direction_.Set(x, y, z);
            Direction_.Normalize();
        }

        Color &DirectionalLight::GetColor()
        {
            return Color_;
        }

        void DirectionalLight::SetColor(const Color &color)
        {
            Color_ = color;
        }

        void DirectionalLight::SetColor(float x, float y, float z)
        {
            Color_ = Color(x, y, z, 1);
        }

        

        ///////////////////////////////////////////////////////////////////
        // point light
        PointLight::PointLight()
        {
            m_position = Vec3(0, 0, 0);
            Color_ = Color(1, 1, 1, 1);
            Radius_ = 5;
        }

        SVec3 &PointLight::GetPosition()
        {
            return m_position;
        }

        void PointLight::SetPosition(const SVec3 &position)
        {
            m_position = position;
        }

        void PointLight::SetPosition(float x, float y, float z)
        {
            m_position = SVec3(x, y, z);
        }

        Color &PointLight::GetColor()
        {
            return Color_;
        }

        void PointLight::SetColor(const Color &color)
        {
            Color_ = color;
        }

        void PointLight::SetColor(float x, float y, float z)
        {
            Color_ = Color(x, y, z, 1);
        }


        float &PointLight::GetRadius()
        {
            return Radius_;
        }

        void PointLight::SetRadius(float radius)
        {
            Radius_ = radius;
        }

        ///////////////////////////////////////////////////////////////////
        // universal light
        void Light::Initialize(void) 
        {
            m_type = LIGHT_DIRECTIONAL;
            m_position = SVec3(0, 0, 0);
            m_color = Color::White;
            m_radius = 1.0f;
            m_direction = SVec3(0, -1, 0);
            m_intensity = 1.0f;;

            m_spotlightAngles = Vec2(30, 30);
        }

        Light::LightType Light::GetType(void)
        {
            return m_type;
        }

        void Light::SetType(const Light::LightType type)
        {
            m_type = type;
        }

        const SVec3& Light::GetDirection(void)
        {
            return m_direction;
        }

        void Light::SetDirection(const SVec3& dir)
        {
            m_direction = dir;
        }

        void Light::SetDirection(const float x, const float y, const float z)
        {
            m_direction = SVec3(x, y, z);
        }

        const SVec3& Light::GetPosition(void)
        {
            return m_position;
        }

        void Light::SetPosition(const SVec3& position)
        {
            m_position = position;
        }

        void Light::SetPosition(const float x, const float y, const float z)
        {
            m_position = SVec3(x, y, z);
        }

        const Color& Light::GetColor(void)
        {
            return m_color;
        }

        void Light::SetColor(const Color& color)
        {
            m_color = color;
        }

        void Light::SetColor(const float r, const float g, const  float b)
        {
            m_color = Color(r, g, b, 1.0f);
        }

        float Light::GetRadius(void)
        {
            return m_radius;
        }

        void Light::SetRadius(const float radius)
        {
            m_radius = radius;
        }

        float Light::GetIntensity()
        {
            return m_intensity;
        }

        void Light::SetIntensity(const float intensity)
        {
            m_intensity = intensity;
        }

        const Vec2& Light::GetSpotlightAngles()
        {
            return m_spotlightAngles;
        }

        void Light::SetSpotlightAngles(const Vec2& angles)
        {
            m_spotlightAngles = angles;
        }

        void Light::SetSpotlightAngles(const float inner, const float outer)
        {
            m_spotlightAngles = Vec2(inner, outer);
        }
    }
}
