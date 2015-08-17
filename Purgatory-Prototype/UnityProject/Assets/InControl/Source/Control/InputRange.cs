using System;
using System.IO;
using UnityEngine;


namespace InControl
{
	/// <summary>
	/// This type represents a range inclusive of two values, and can remap a value from one range to another.
	/// </summary>
	public struct InputRange
	{
		public static readonly InputRange None = new InputRange( 0.0f, 0.0f, InputRangeType.None );
		public static readonly InputRange MinusOneToOne = new InputRange( -1.0f, 1.0f, InputRangeType.MinusOneToOne );
		public static readonly InputRange ZeroToOne = new InputRange( 0.0f, 1.0f, InputRangeType.ZeroToOne );
		public static readonly InputRange ZeroToMinusOne = new InputRange( 0.0f, -1.0f, InputRangeType.ZeroToMinusOne );

		// TODO: These should be deprecated when custom profiles are.
		public static readonly InputRange ZeroToNegativeInfinity = new InputRange( 0.0f, float.NegativeInfinity, InputRangeType.ZeroToNegativeInfinity );
		public static readonly InputRange ZeroToPositiveInfinity = new InputRange( 0.0f, float.PositiveInfinity, InputRangeType.ZeroToPositiveInfinity );
		public static readonly InputRange Everything = new InputRange( float.NegativeInfinity, float.PositiveInfinity, InputRangeType.Everything );

		static readonly InputRange[] TypeToRange = new InputRange[] {
			None,
			MinusOneToOne,
			ZeroToOne,
			ZeroToMinusOne,
			ZeroToNegativeInfinity,
			ZeroToPositiveInfinity,
			Everything
		};


		/// <summary>
		/// The first value in the range.
		/// </summary>
		public readonly float Value0;

		/// <summary>
		/// The second value in the range.
		/// </summary>
		public readonly float Value1;

		/// <summary>
		/// An enum type of the range.
		/// </summary>
		public readonly InputRangeType Type;


		/// <summary>
		/// Initializes a new range from two given values.
		/// </summary>
		/// <param name="value0">The first value in the range.</param>
		/// <param name="value1">The second value in the range.</param>
		InputRange( float value0, float value1, InputRangeType type )
		{
			Value0 = value0;
			Value1 = value1;
			Type = type;
		}


		/// <summary>
		/// Initializes a new range from a predefined type.
		/// </summary>
		public InputRange( InputRangeType type )
		{
			Value0 = TypeToRange[(int) type].Value0;
			Value1 = TypeToRange[(int) type].Value1;
			Type = type;
		}


		/// <summary>
		/// Check whether a value falls within of this range.
		/// </summary>
		/// <returns><c>true</c>, if the value falls within this range, <c>false</c> otherwise.</returns>
		/// <param name="value">The value to check.</param>
		public bool Includes( float value )
		{
			return !Excludes( value );
		}


		/// <summary>
		/// Check whether a value falls outside of this range.
		/// </summary>
		/// <returns><c>true</c>, if the value falls outside this range, <c>false</c> otherwise.</returns>
		/// <param name="value">The value to check.</param>
		public bool Excludes( float value )
		{
			if (Type == InputRangeType.None)
			{
				return true;
			}

			return (value < Mathf.Min( Value0, Value1 ) || value > Mathf.Max( Value0, Value1 ));
		}


		/// <summary>
		/// Remap the specified value, from one range to another.
		/// </summary>
		/// <param name="value">The value to remap.</param>
		/// <param name="sourceRange">The source range to map from.</param>
		/// <param name="targetRange">The target range to map to.</param>
		public static float Remap( float value, InputRange sourceRange, InputRange targetRange )
		{
			if (sourceRange.Excludes( value ))
			{
				return 0.0f;
			}
			var sourceValue = Mathf.InverseLerp( sourceRange.Value0, sourceRange.Value1, value );
			return Mathf.Lerp( targetRange.Value0, targetRange.Value1, sourceValue ); 
		}


		internal static float Remap( float value, InputRangeType sourceRangeType, InputRangeType targetRangeType )
		{
			var sourceRange = InputRange.TypeToRange[(int) sourceRangeType];
			var targetRange = InputRange.TypeToRange[(int) targetRangeType];
			return Remap( value, sourceRange, targetRange );
		}
	}
}
