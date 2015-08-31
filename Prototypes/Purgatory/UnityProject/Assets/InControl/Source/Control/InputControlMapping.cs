using System;
using UnityEngine;

namespace InControl
{
	public class InputControlMapping
	{
		// [TinyJSON.TypeHint]
		public InputControlSource Source;

		public InputControlType Target;

		// Invert the final mapped value.
		public bool Invert = false;

		// Analog values will be multiplied by this number before processing.
		public float Scale = 1.0f;

		// Raw inputs won't be processed except for scaling (mice and trackpads).
		public bool Raw = false;

		// This is primarily to fix a bug with the wired Xbox controller on Mac.
		public bool IgnoreInitialZeroValue = false;

		// Profile settings can be overriden with these.
		public float Sensitivity = 1.0f;
		public float LowerDeadZone = 0.0f;
		public float UpperDeadZone = 1.0f;

		public InputRange SourceRange = InputRange.MinusOneToOne;
		public InputRange TargetRange = InputRange.MinusOneToOne;

		string handle;


		public float MapValue( float value )
		{
			if (Raw)
			{
				value = value * Scale;
				value = SourceRange.Excludes( value ) ? 0.0f : value;
			}
			else
			{
				// Scale value and clamp to a legal range.
				value = Mathf.Clamp( value * Scale, -1.0f, 1.0f );

				// Remap from source range to target range.
				value = InputRange.Remap( value, SourceRange, TargetRange );
			}

			if (Invert)
			{
				value = -value;
			}

			return value;
		}


		public string Handle
		{
			get { return (string.IsNullOrEmpty( handle )) ? Target.ToString() : handle; }
			set { handle = value; }
		}
	}
}
