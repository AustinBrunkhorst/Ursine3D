/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TextDataStructures.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct InfoData
{
    int16_t     fontSize;
    uint8_t     bitField;
    uint8_t     characterSet;
    uint16_t    stretchHorizontal;
    uint8_t     superSamplingLevel;
    uint8_t     paddingUp;
    uint8_t     paddingRight;
    uint8_t     paddingDown;
    uint8_t     paddingLeft;

    struct
    {
        uint8_t     spacingHorizontal;
        uint8_t     spacingVertical;
    } spacing;
    
    uint8_t     outlineThickness;
    std::string fontName;
};

struct CommonData
{
    uint16_t    lineHeight;
    uint16_t    base;

    struct
    {
        float    scaleW;
        float    scaleH;
    } textureDimensions;

    uint16_t    pages;
    uint8_t     bitField;

    struct
    {
        uint8_t     alphaChannel;
        uint8_t     redChannel;
        uint8_t     greenChannel;
        uint8_t     blueChannel;
    } colorChannels;
};

struct CharacterData
{
    uint32_t    id;
    struct
    {
        float x;
        float y;
    } textureCoordinates;

    struct
    {
        float x;
        float y;
    } textureDimensions;

    struct
    {
        int16_t     x;
        int16_t     y;
    } offset;
    
    int16_t     xadvance;
    uint32_t    page;
    uint32_t    chnl;

    std::unordered_map<unsigned, int16_t> kerningMap;
};
