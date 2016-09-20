#pragma once

namespace ursine
{
    // used to grab textures
    enum TEX_TYPE
    {
        TEX_DIFFUSE = 0,
        TEX_NORMAL,
        TEX_BUMP,
        TEX_COUNT
    };

    enum LIGHTING_MODEL
    {
        LIGHTING_MODEL_PHONG,
        LIGHTING_MODEL_MF_BRDF,
        LIGHTING_MODEL_COUNT
    };

    // material data
    struct GxMaterial
    {
        // lighting data, not how much we need but this covers the bases
        SVec3 emissive;                         // emissive RGB
        float emissiveStrength;                 // emissive strength

        SVec3 ambient;                          // ambient RGB
        float ambientStrength;                  // ambient strength

        SVec3 diffuse;                          // diffuse RGB
        float diffuseStrength;                  // diffuse strength

        LIGHTING_MODEL lightingModel;

        // if we want to support both of them? not sure
        union
        {
            // phong data
            struct
            {
                // specularity
                float specularAngle;
                float specularIntensity;
            } phongInfo;

            struct
            {
                SVec3 baseColor;
                float roughness;
                float metallic;
                float specular;
            } pbrInfo;
        } lightingData;

        // textures
        std::string texturePaths[ TEX_COUNT ];  // paths to our textures
    };
};
