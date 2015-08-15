#pragma once

namespace Ursine
{
    struct BlendMode
    {
        enum Factor
        {
            // (0, 0, 0, 0)
            Zero,
            // (1, 1, 1, 1)
            One,
            // (src.r, src.g, src.b, src.a)
            SrcColor,
            // (1, 1, 1, 1) - (src.r, src.g, src.b, src.a)
            OneMinusSrcColor,
            // (dst.r, dst.g, dst.b, dst.a)
            DstColor,
            // (1, 1, 1, 1) - (dst.r, dst.g, dst.b, dst.a)
            OneMinusDstColor,
            // (src.a, src.a, src.a, src.a)
            SrcAlpha,
            // (1, 1, 1, 1) - (src.a, src.a, src.a, src.a)
            OneMinusSrcAlpha,
            // (dst.a, dst.a, dst.a, dst.a)
            DstAlpha,
            // (1, 1, 1, 1) - (dst.a, dst.a, dst.a, dst.a)
            OneMinusDstAlpha
        };

        enum Equation
        {
            // Pixel = Src * SrcFactor + Dst * DstFactor
            Add,
            // Pixel = Src * SrcFactor - Dst * DstFactor
            Subtract
        };

        BlendMode(void);

        BlendMode(Factor sourceFactor, 
            Factor destinationFactor, 
            Equation blendEquation = Add);

        BlendMode(Factor colorSourceFactor, 
            Factor colorDestinationFactor,
            Equation colorBlendEquation, 
            Factor alphaSourceFactor,
            Factor alphaDestinationFactor, 
            Equation alphaBlendEquation);

        // Source blending factor for the color channels
        Factor colorSrcFactor;

        // Destination blending factor for the color channels
        Factor colorDstFactor;

        // Blending equation for the color channels
        Equation colorEquation;

        // Source blending factor for the alpha channel
        Factor alphaSrcFactor;

        // Destination blending factor for the alpha channel
        Factor alphaDstFactor;

        // Blending equation for the alpha channel
        Equation alphaEquation;
    };

    bool operator==(const BlendMode &left, const BlendMode &right);
    bool operator!=(const BlendMode &left, const BlendMode &right);

    ////////////////////////////////////////////////////////////////////////////
    // Commonly Used
    ////////////////////////////////////////////////////////////////////////////

    // Blend source and dest according to dest alpha 
    extern const BlendMode BlendAlpha;

    // Add source to dest
    extern const BlendMode BlendAdd;

    // Multiply source and dest
    extern const BlendMode BlendMultiply;

    // Overwrite dest with source
    extern const BlendMode BlendNone;
}
