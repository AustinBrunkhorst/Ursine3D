/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Color.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineTypes.h"

#include "SVec4.h"

#include "JsonSerializer.h"

#include "Utils.h"

namespace ursine
{
    struct Color
    {
        float r, g, b, a;

#pragma region Predefined Colors
        static const Color TransparentBlack;
        static const Color Transparent;
        static const Color AliceBlue;
        static const Color AntiqueWhite;
        static const Color Aqua;
        static const Color Aquamarine;
        static const Color Azure;
        static const Color Beige;
        static const Color Bisque;
        static const Color Black;
        static const Color BlanchedAlmond;
        static const Color Blue;
        static const Color BlueViolet;
        static const Color Brown;
        static const Color BurlyWood;
        static const Color CadetBlue;
        static const Color Chartreuse;
        static const Color Chocolate;
        static const Color Coral;
        static const Color CornflowerBlue;
        static const Color Cornsilk;
        static const Color Crimson;
        static const Color Cyan;
        static const Color DarkBlue;
        static const Color DarkCyan;
        static const Color DarkGoldenrod;
        static const Color DarkGray;
        static const Color DarkGreen;
        static const Color DarkKhaki;
        static const Color DarkMagenta;
        static const Color DarkOliveGreen;
        static const Color DarkOrange;
        static const Color DarkOrchid;
        static const Color DarkRed;
        static const Color DarkSalmon;
        static const Color DarkSeaGreen;
        static const Color DarkSlateBlue;
        static const Color DarkSlateGray;
        static const Color DarkTurquoise;
        static const Color DarkViolet;
        static const Color DeepPink;
        static const Color DeepSkyBlue;
        static const Color DimGray;
        static const Color DodgerBlue;
        static const Color Firebrick;
        static const Color FloralWhite;
        static const Color ForestGreen;
        static const Color Fuchsia;
        static const Color Gainsboro;
        static const Color GhostWhite;
        static const Color Gold;
        static const Color Goldenrod;
        static const Color Gray;
        static const Color Green;
        static const Color GreenYellow;
        static const Color Honeydew;
        static const Color HotPink;
        static const Color IndianRed;
        static const Color Indigo;
        static const Color Ivory;
        static const Color Khaki;
        static const Color Lavender;
        static const Color LavenderBlush;
        static const Color LawnGreen;
        static const Color LemonChiffon;
        static const Color LightBlue;
        static const Color LightCoral;
        static const Color LightCyan;
        static const Color LightGoldenrodYellow;
        static const Color LightGray;
        static const Color LightGreen;
        static const Color LightPink;
        static const Color LightSalmon;
        static const Color LightSeaGreen;
        static const Color LightSkyBlue;
        static const Color LightSlateGray;
        static const Color LightSteelBlue;
        static const Color LightYellow;
        static const Color Lime;
        static const Color LimeGreen;
        static const Color Linen;
        static const Color Magenta;
        static const Color Maroon;
        static const Color MediumAquamarine;
        static const Color MediumBlue;
        static const Color MediumOrchid;
        static const Color MediumPurple;
        static const Color MediumSeaGreen;
        static const Color MediumSlateBlue;
        static const Color MediumSpringGreen;
        static const Color MediumTurquoise;
        static const Color MediumVioletRed;
        static const Color MidnightBlue;
        static const Color MintCream;
        static const Color MistyRose;
        static const Color Moccasin;
        static const Color NavajoWhite;
        static const Color Navy;
        static const Color OldLace;
        static const Color Olive;
        static const Color OliveDrab;
        static const Color Orange;
        static const Color OrangeRed;
        static const Color Orchid;
        static const Color PaleGoldenrod;
        static const Color PaleGreen;
        static const Color PaleTurquoise;
        static const Color PaleVioletRed;
        static const Color PapayaWhip;
        static const Color PeachPuff;
        static const Color Peru;
        static const Color Pink;
        static const Color Plum;
        static const Color PowderBlue;
        static const Color Purple;
        static const Color Red;
        static const Color RosyBrown;
        static const Color RoyalBlue;
        static const Color SaddleBrown;
        static const Color Salmon;
        static const Color SandyBrown;
        static const Color SeaGreen;
        static const Color SeaShell;
        static const Color Sienna;
        static const Color Silver;
        static const Color SkyBlue;
        static const Color SlateBlue;
        static const Color SlateGray;
        static const Color Snow;
        static const Color SpringGreen;
        static const Color SteelBlue;
        static const Color Tan;
        static const Color Teal;
        static const Color Thistle;
        static const Color Tomato;
        static const Color Turquoise;
        static const Color Violet;
        static const Color Wheat;
        static const Color White;
        static const Color WhiteSmoke;
        static const Color Yellow;
        static const Color YellowGreen;
        #pragma endregion

        // Default constructor (white)
        Color(void);

        // Constructs a color from the respective red, green, and blue values
        Color(float red, float green, float blue, float alpha);

        // Constructs a color from an existing color and alpha value
        Color(const Color &color, float alpha);

        // Constructs a color from a vector3 (r, g, b)
        Color(const SVec3 &value);
        
        // Constructs a color from a vector4 (r, g, b, a)
        Color(const SVec4 &value);

        // Constructs a color from a hexadecimal value: 0xAARRGGBB
        Color(uint32 hex);

        // Red value from 0 to 255
        byte ByteR(void) const;

        // Green value from 0 to 255
        byte ByteG(void) const;

        // Blue value from 0 to 255
        byte ByteB(void) const;

        // Alpha value from 0 to 255
        byte ByteA(void) const;

        // Converts the color to a SVec3 (r, g, b)
        SVec3 ToVector3(void) const;

        // Converts the color to a SVec4 (r, g, b, a)
        SVec4 ToVector4(void) const;

        // Converts the color to its hexadecimal representation
        uint32 ToHex(void) const;
    } Meta(Enable, WhiteListMethods);

    extern template
    Json JsonSerializer::Serialize(Color &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, Color &out);

    extern template
    Json JsonSerializer::Serialize(std::vector<Color> &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, std::vector<Color> &out);

    namespace math
    {
        extern template
        Color Lerp(const Color &a, const Color &b, float percent);
    }
}