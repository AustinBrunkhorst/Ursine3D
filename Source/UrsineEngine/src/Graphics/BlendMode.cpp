#include "UrsinePrecompiled.h"

#include "BlendMode.h"

namespace Ursine
{
    ////////////////////////////////////////////////////////////////////////////
    // Commonly Used
    ////////////////////////////////////////////////////////////////////////////

    const BlendMode BlendAlpha(
        BlendMode::SrcAlpha,
        BlendMode::OneMinusSrcAlpha,
        BlendMode::Add,
        BlendMode::One,
        BlendMode::OneMinusSrcAlpha,
        BlendMode::Add );

    const BlendMode BlendAdd(
        BlendMode::SrcAlpha,
        BlendMode::One,
        BlendMode::Add,
        BlendMode::One,
        BlendMode::One,
        BlendMode::Add );

    const BlendMode BlendMultiply(
        BlendMode::DstColor,
        BlendMode::Zero);

    const BlendMode BlendNone(
        BlendMode::One,
        BlendMode::Zero);

    ////////////////////////////////////////////////////////////////////////////
    // Constructors
    ////////////////////////////////////////////////////////////////////////////

    BlendMode::BlendMode(void)
        : colorSrcFactor( SrcAlpha )
        , colorDstFactor( OneMinusSrcAlpha )
        , colorEquation( Add )
        , alphaSrcFactor( One )
        , alphaDstFactor( OneMinusSrcAlpha )
        , alphaEquation( Add ) { }

    BlendMode::BlendMode(Factor sourceFactor, Factor destinationFactor, Equation blendEquation) 
        : colorSrcFactor( sourceFactor )
        , colorDstFactor( destinationFactor )
        , colorEquation( blendEquation )
        , alphaSrcFactor( sourceFactor )
        , alphaDstFactor( destinationFactor )
        , alphaEquation( blendEquation ) { }

    BlendMode::BlendMode(Factor colorSourceFactor, 
        Factor colorDestinationFactor,
        Equation colorBlendEquation, 
        Factor alphaSourceFactor,
        Factor alphaDestinationFactor, 
        Equation alphaBlendEquation) 
        : colorSrcFactor( colorSourceFactor )
        , colorDstFactor( colorDestinationFactor )
        , colorEquation( colorBlendEquation )
        , alphaSrcFactor( alphaSourceFactor )
        , alphaDstFactor( alphaDestinationFactor )
        , alphaEquation( alphaBlendEquation ) { }

    ////////////////////////////////////////////////////////////////////////////
    // Operators
    ////////////////////////////////////////////////////////////////////////////
    
    bool operator==(const BlendMode &left, const BlendMode &right)
    {
        return (left.colorSrcFactor == right.colorSrcFactor) &&
               (left.colorDstFactor == right.colorDstFactor) &&
               (left.colorEquation == right.colorEquation) &&
               (left.alphaSrcFactor == right.alphaSrcFactor) &&
               (left.alphaDstFactor == right.alphaDstFactor) &&
               (left.alphaEquation == right.alphaEquation);
    }

    bool operator!=(const BlendMode &left, const BlendMode &right)
    {
        return !(left == right);
    }
}
