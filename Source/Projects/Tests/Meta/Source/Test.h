/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Test.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Meta.h>

///////////////////////////////////////////////////////////////////////////////
// Classes
///////////////////////////////////////////////////////////////////////////////

class Range : public ursine::meta::MetaProperty
{   
    META_OBJECT 
public:
    Range(int min, int max) 
        : min( min )
        , max( max ) { }

    int min;
    int max; 
};

enum SliderType
{
    Horizontal,
    Vertical
};

class Slider : public ursine::meta::MetaProperty
{
    META_OBJECT
public:
    Slider(void) 
        : type( Horizontal ) { }

    Slider(SliderType type) 
        : type( type ) { }

    SliderType type;
};

class Jason : public ursine::meta::Object
{
    META_OBJECT
public:
    Jason(void) 
        : myField( 0 ) { }

    Meta(Range( -3000, 50 ), Slider)
    int myField;
};

class BaseClass { };

class Test : public BaseClass
{
public:
	double publicField;
	double publicField2;

    Meta(Enable)
    static const int StaticField = 5;

    static const int StaticField2 = 6;

    Test(void) { }
    Test(int, double, float) { }
   
	void ConstMethod(void) const { }

	void OverloadedMethod(void) { }
	void OverloadedMethod(int) { }
	void OverloadedMethod(int, int) { }
	void OverloadedMethod(int, int) const { }

    Meta(Enable)
    static int StaticMethod(void) { return 0; }

private:
	int m_privateField;

	Test(int, int) { }

	void privateMethod(void) { }
};

class Declaration;

class Disabled { };

///////////////////////////////////////////////////////////////////////////////
// Enums
///////////////////////////////////////////////////////////////////////////////

enum Enum 
{
	One,
	Two,
	Three
};

enum class EnumClass : unsigned
{
	Four,
	Five,
	Six
};