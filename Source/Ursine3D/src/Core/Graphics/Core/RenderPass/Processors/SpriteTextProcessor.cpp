/* ----------------------------------------------------------------------------
** Team Bear King
** ? 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SpriteTextProcessor.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/
#include "UrsinePrecompiled.h"

#include "SpriteTextProcessor.h"
#include "Renderable.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        SpriteTextProcessor::SpriteTextProcessor(bool overdraw)
            : m_overdraw( overdraw )
        {
            m_renderableType = RENDERABLE_SPRITE_TEXT;
        }

        bool SpriteTextProcessor::cullOperation(_DRAWHND handle, Camera &currentCamera)
        {
            UAssert(handle.Type_ == m_renderableType, "GfxEntityProcessor attempted to proces invalid type!");

            SpriteText &spriteText = m_manager->renderableManager->GetRenderableByID<SpriteText>( handle.Index_ );

            return true;
            // if inactive
            if (!spriteText.GetActive())
                return true;

            // if culed by camera mask
            if (currentCamera.CheckMask( spriteText.GetRenderMask( ) ))
                return true;

            // if no text
            if (spriteText.GetText( ).length( ) <= 0)
                return true;

            // if no font
            if(spriteText.GetFontHandle( ) == 0)
                return true;

            // if not matching overdraw
            if(spriteText.GetOverdraw( ) != m_overdraw)
                return true;

            // return false as in DO NOT CULL ME
            return false;
        }

        void SpriteTextProcessor::prepOperation(_DRAWHND handle, SMat4 &view, SMat4 &proj, Camera &currentCamera)
        {
            SpriteText &spriteText = m_manager->renderableManager->GetRenderableByID<SpriteText>( handle.Index_ );

            // map color data ///////////////////////////////////////
            PointGeometryBuffer pgb;
            auto &color = spriteText.GetColor( );
            pgb.cameraUp.x = color.r;
            pgb.cameraUp.y = color.g;
            pgb.cameraUp.z = color.b;
            pgb.cameraUp.w = color.a;
            m_manager->bufferManager->MapBuffer<BUFFER_POINT_GEOM>(
                &pgb, 
                SHADERTYPE_VERTEX
            );

            // map ID data //////////////////////////////////////////
            MaterialDataBuffer mdb;
            int overdrw = spriteText.GetOverdraw( ) == true ? 1 : 0;
            mdb.id = (handle.Index_) | (handle.Type_ << 12) | (overdrw << 15) | (1 << 11);
            m_manager->bufferManager->MapBuffer<BUFFER_MATERIAL_DATA>(
                &mdb, 
                SHADERTYPE_PIXEL
            );

            // change sampler ///////////////////////////////////////
            if (spriteText.GetFilter( ))
                m_manager->textureManager->MapSamplerState( SAMPLER_STATE_WRAP_TEX );
            else
                m_manager->textureManager->MapSamplerState( SAMPLER_STATE_NO_FILTERING );

            mapSpriteTextBuffer( handle );
        }

        void SpriteTextProcessor::renderOperation(_DRAWHND handle, Camera &currentCamera)
        {
            SpriteText &spriteText = m_manager->renderableManager->GetRenderableByID<SpriteText>( handle.Index_ );
            m_manager->shaderManager->Render( 6 * static_cast<unsigned>( spriteText.GetText( ).length( ) ) );
        }

        void SpriteTextProcessor::mapSpriteTextBuffer(_DRAWHND handle)
        {
            SpriteText &spriteText = m_manager->renderableManager->GetRenderableByID<SpriteText>( handle.Index_ );

            // map text data ////////////////////////////////////////
            SpriteTextBuffer stb;
            auto &font = m_manager->fontManager->GetBitmapFont(handle.Material_);
            stb.worldPosition = spriteText.GetPosition( ).ToD3D( );
            stb.offset = spriteText.GetSize( );
            stb.sizeScalar = DirectX::XMFLOAT2(
                spriteText.GetWidth( ) * spriteText.GetSize( ),
                spriteText.GetHeight( ) * spriteText.GetSize( )
            );

            /////////////////////////////////////////////////////////
            // mapping glyph data
            Vec2 cursor = Vec2(0, 0);   // "cursor" for text positions

            GlyphBuffer gb;
            auto &text = spriteText.GetText( );
            auto &characterData = font.GetCharacterData( );
            auto &commonData = font.GetCommonData( );
            auto &infoData = font.GetInfoData( );

            stb.textureDimensions = DirectX::XMFLOAT2(
                commonData.textureDimensions.scaleW,
                commonData.textureDimensions.scaleH
            );

            // map the texture
            auto texHandle = m_manager->fontManager->GetTextureHandle( spriteText.GetFontHandle( ), font.GetPageData( )[0] );
            m_manager->textureManager->MapTextureByID( texHandle & 0xFFFF );
            
            // offset data for calculating stuff
            float distance = 0;
            unsigned lineStartIndex = 0;

            // for each glyph, determine position and stuff
            for (unsigned x = 0; x < text.length( ); ++x)
            {
                auto &currentCharacter = text[ x ];
                auto &currentGlyph = gb.glyphData[ x ];

                auto &currentCharData = characterData[ currentCharacter ];

                // for newline characters
                if (currentCharacter == '\n')
                {
                    // calculate the current offsets so we can center ourselves
                    float lineOffset;

                    switch (spriteText.GetAlignment( ))
                    {
                    case 1:
                        lineOffset = ( distance / commonData.textureDimensions.scaleW ) * 0.5f;
                        break;
                    case 2:
                        lineOffset = distance / commonData.textureDimensions.scaleW;
                        break;
                    default:
                        lineOffset = 0;
                        break;
                    }

                    // add it to the current line's characters
                    for (unsigned y = lineStartIndex; y < x; ++y)
                    {
                        gb.glyphData[ y ].screenPosition.x -= lineOffset;
                    }

                    // reset the stats, don't include the current newline
                    lineStartIndex = x + 1;
                    distance = 0;

                    // increment y
                    cursor += Vec2( 0, ( commonData.lineHeight / commonData.textureDimensions.scaleH ) * stb.sizeScalar.y );

                    continue;
                }

                // First, advance cursor by the xoffset
                cursor += Vec2(
                    currentCharData.offset.x, 
                    0
                );

                // then, check if we need to kern
                if (x > 0)
                {   // grab kerning data for behind us. Check if we need to apply
                    auto kerningData = characterData[ text[ x - 1 ] ].kerningMap.find( static_cast<int16_t>( currentCharacter ) );

                    // if the last character has us
                    if (kerningData != characterData[ text[ x - 1 ] ].kerningMap.end( ))
                    {
                        // add the kerning data to us
                        cursor += Vec2(kerningData->second, 0);
                    }
                }

                // "render", divide by size of the tex to scale to right size
                currentGlyph.screenPosition = DirectX::XMFLOAT2( cursor.X( ) / commonData.textureDimensions.scaleW, cursor.Y( ) );

                currentGlyph.buffer.x = spriteText.GetPPU( );

                // set glyph position in tex
                currentGlyph.glyphPosition = DirectX::XMFLOAT2(
                    currentCharData.textureCoordinates.x / commonData.textureDimensions.scaleW,
                    currentCharData.textureCoordinates.y / commonData.textureDimensions.scaleH
                );

                // set glyph size
                currentGlyph.glyphSize = DirectX::XMFLOAT2(
                    currentCharData.textureDimensions.x / commonData.textureDimensions.scaleW,
                    currentCharData.textureDimensions.y / commonData.textureDimensions.scaleH
                );

                cursor += Vec2(
                    currentCharData.xadvance, 
                    0
                );

                distance = cursor.X( );
            }

            // perform final line adjustment
            // calculate the current offsets so we can center ourselves
            float lineOffset;

            switch (spriteText.GetAlignment( ))
            {
            case 1:
                lineOffset = (distance / commonData.textureDimensions.scaleW) * 0.5f;
                break;
            case 2:
                lineOffset = distance / commonData.textureDimensions.scaleW;
                break;
            default:
                lineOffset = 0;
                break;
            }

            // add it to the current line's characters
            for (unsigned y = lineStartIndex; y < text.length( ); ++y)
            {
                gb.glyphData[ y ].screenPosition.x -= lineOffset;
            }

            // DONE, map the data
            m_manager->bufferManager->MapBuffer<BUFFER_TEXTDATA>(
                &stb, 
                SHADERTYPE_VERTEX, 
                7
            );
            m_manager->bufferManager->MapBuffer<BUFFER_GLYPHDATA>(
                &gb, 
                SHADERTYPE_VERTEX, 
                10
            );
        }
    }
}