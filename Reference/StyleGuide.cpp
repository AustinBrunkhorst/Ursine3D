/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** <FileName.extension>
**
** Author:
** - <First> <Last> - f.last@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

// lowercase, words delimited by underscore (no camel case)
namespace namespace_name
{
	// uppercase, camel case
	class ClassName
		, DerivedClass1
		, DerivedClass2
		, LastDerivedClass
	{
	// public should always come first in header declarations
	public:
		// lowercase, camel case
		double publicField;

		ClassName(void);

		// void must be included in no-argument functions
		void PublicMethod(void) const;

		// & and * couple argument names
		int PublicMethod(const int &arg1, double *arg2);

		// example function header comment - basic doxygen

		/** @brief Prints the string str, starting at the current
		 *         location of the cursor.
		 *
		 *  If the string is longer than the current line, the
		 *  string should fill up the current line and then
		 *  continue on the next line. If the string exceeds
		 *  available space on the entire console, the screen
		 *  should scroll up one line, and then the string should
		 *  continue on the new line.  If '\n', '\r', and '\b' are
		 *  encountered within the string, they should be handled
		 *  as per putbyte. If length is not a positive integer or str
		 *  is null, the function has no effect.
		 *
		 *  @param str The string to be printed.
		 *  @param length The length of the string str.
		 *  @return Void.
		 */
		void PutBytes(const char *str, int length);

	// protected follows the same style as private
	private:
		// same as public, but prefixed with "m_"
		int m_privateField;

		std::string m_anotherPrivate;

		// same as public fields
		void privateMethod(void);
	};

	// constructor initializer lists should follow this format
	ClassName::ClassName(void)
		: publicField( 3.2 )
		, m_privateField( 4 )
		, m_anotherPrivate( "spacing, man." ) 
	{

	}
}

// global functions follow the same naming convention as classes
void GlobalFunction(void) 
{ 
	bool test = false;

	if (test) 
	{
		// braces are on newlines
	}

	// all language constructs have spaces after the keyword and 
	// before the open parentheses

	for (;;);
	while (true);
	do { } while (true);
}

// please keep comments lowercase
// <-- also keep one space after comment declarations
int main(void)
{
	// always use "f" when describing float literals
	float decimal = 19.45f;

	// always keep at least a 0 for the fractional part of any floating point
	double otherDecimal = 15.0; // 15, 15., 15.f : no no

	namespace_name::ClassName myClass;

	// function invocation arguments are padded
	// with spaces

	myClass.PublicMethod( );

	myClass.PublicMethod( 1, &otherDecimal );

	int justAnArray[10];

	// index operators are also padded
	int value = justAnArray[ 3 ];

	// prefer prefix increment/decrement over postfix
	for (int i = 0; i < 10; ++i) { }

	// prefer initializer list constructors
	std::vector<int> justAVector { 1, 2, 3, 4 };

	// prefer range-based for loops when possible
	for (auto value : justAVector) { }

	// absolutely use "auto" type deduction when
	// possible.
	auto anotherValue = justAVector[ 2 ];

	// use */& coupled with these types
	// when using auto
	auto *ptrValue = &justAVector[ 0 ];
	auto &refValue = justAVector[ 1 ];

	// use a lot of newlines so sections of code don't get too cluttered

	for (auto value : justAVector)
	{
		if (value >= 5 && *ptrValue == 3)
		{
			myClass.PublicMethod( );

			justAnArray[ 3 ] = 3;

			justAVector.push_back( 9001 );
		}

		refValue = 6;
	}

	// UNDESIRABLE

	for (auto value : justAVector)
	{
		if (value >= 5 && *ptrValue == 3)
		{
			myClass.PublicMethod( );
			justAnArray[ 3 ] = 3;
			justAVector.push_back( 9001 );
		}
		refValue = 6;
	}

	// treat your code as a piece of art, because it is.
	// we're going for consistency in addition to readability
}