#pragma once

#include <vector>
#include <string>
#include <queue>
#include <memory>
#include <unordered_map>
#include <fbxsdk.h>
#include <iostream>

namespace pseudodx
{
    struct XMFLOAT4
    {
        float x;
        float y;
        float z;
        float w;
        XMFLOAT4( ) {}
        XMFLOAT4( float _x, float _y, float _z, float _w ) : x( _x ), y( _y ), z( _z ), w( _w ) {}
        XMFLOAT4& operator= ( const XMFLOAT4& Float4 ) { x = Float4.x; y = Float4.y; z = Float4.z; w = Float4.w; return *this; }
    };

    struct XMFLOAT3
    {
        float x;
        float y;
        float z;

        XMFLOAT3( ) {}
        XMFLOAT3( float _x, float _y, float _z ) : x( _x ), y( _y ), z( _z ) {}
        XMFLOAT3& operator= ( const XMFLOAT3& Float3 ) { x = Float3.x; y = Float3.y; z = Float3.z; return *this; }
    };

    struct XMFLOAT2
    {
        float x;
        float y;

        XMFLOAT2( ) {}
        XMFLOAT2( float _x, float _y ) : x( _x ), y( _y ) {}
        XMFLOAT2& operator= ( const XMFLOAT2& Float2 ) { x = Float2.x; y = Float2.y; return *this; }
    };
}

namespace ursine
{
    namespace graphics
    {
        namespace FBX_DATA
        {
            // UVSet
            typedef std::tr1::unordered_map<std::string, int> UVsetID;
            // UVSet
            typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureSet;
            
            struct Material_Consts
            {
                pseudodx::XMFLOAT4	ambient;
                pseudodx::XMFLOAT4	diffuse;
                pseudodx::XMFLOAT4	specular;
                pseudodx::XMFLOAT4	emissive;
                float		shineness;
                float		TransparencyFactor;
            };

            struct ModelSubset
            {
                ModelSubset( ) : id( -1 ), vertexStart( 0 ), vertexCount( 0 ) {};
                //id number of this subset
                int         id;

                //location of where this subset starts and end
                unsigned    vertexStart;
                unsigned    vertexCount;
            };

            struct Material_Eles
            {
                // determine if material only holds material or only textures
                // or both
                enum eMaterial_Fac
                {
                    Fac_None = 0,
                    Fac_Only_Color,
                    Fac_Only_Texture,
                    Fac_Both,
                    Fac_Max,
                };
                eMaterial_Fac type;
                pseudodx::XMFLOAT4 color;
                TextureSet textureSetArray;

                Material_Eles( )
                    :type( Fac_None ), color( 0, 0, 0, 1 )
                {
                    textureSetArray.clear( );
                }

                ~Material_Eles( )
                {
                    Release( );
                }

                void Release( )
                {
                    for ( TextureSet::iterator it = textureSetArray.begin( ); it != textureSetArray.end( ); ++it )
                    {
                        it->second.clear( );
                    }
                    textureSetArray.clear( );
                }

                Material_Eles& operator=( const Material_Eles& rhs )
                {
                    type = rhs.type;
                    color = rhs.color;

                    for ( auto iter = rhs.textureSetArray.begin( ); iter != rhs.textureSetArray.end( ); ++iter )
                    {
                        for ( auto iter2 = iter->second.begin( ); iter2 != iter->second.end( ); ++iter2 )
                        {
                            textureSetArray[ iter->first ].push_back( *iter2 );
                        }
                    }
                    return *this;
                }
            };

            struct FbxMaterial
            {
                enum eMaterial_Type
                {
                    Type_None = 0,
                    Type_Lambert,
                    Type_Phong,
                    Type_Max
                };

                std::string  name;
                eMaterial_Type type;
                // ambiet material and texture
                Material_Eles ambient;
                // diffuse material and texture
                Material_Eles diffuse;
                // emmisive material and texture
                Material_Eles emissive;
                // specular material and texture
                Material_Eles specular;
                float shineness;
                float TransparencyFactor;
                Material_Consts mtrl_consts;

                FbxMaterial( )
                    :name( "" ), type( Type_None ),
                    shineness( 0 ), TransparencyFactor( 0 )
                {}

                void Release( )
                {
                    ambient.Release( );
                    diffuse.Release( );
                    emissive.Release( );
                    specular.Release( );
                }

                FbxMaterial& operator=( const FbxMaterial& rhs )
                {
                    name = rhs.name;
                    type = rhs.type;
                    ambient = rhs.ambient;
                    diffuse = rhs.diffuse;
                    emissive = rhs.emissive;
                    specular = rhs.specular;
                    shineness = rhs.shineness;
                    TransparencyFactor = rhs.TransparencyFactor;

                    mtrl_consts.ambient = rhs.ambient.color;
                    mtrl_consts.diffuse = rhs.diffuse.color;
                    mtrl_consts.emissive = rhs.emissive.color;
                    mtrl_consts.specular = rhs.specular.color;
                    //mtrl_consts.shineness = rhs.shineness;
                    //mtrl_consts.TransparencyFactor = rhs.TransparencyFactor;
                    return *this;
                }
            };

            struct KeyFrame
            {
                float time;
                pseudodx::XMFLOAT4 trans;
                pseudodx::XMFLOAT4 rot;
            };

            struct BoneAnimation
            {
                std::vector<KeyFrame> keyFrames;
            };

            /////////////////////////////////////////////////////////
        }
    }
}