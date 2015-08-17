using System;


namespace InControl
{
	/// <summary>
	/// An enumeration of input controls.
	/// This includes both the standardized set of controls and a variety 
	/// of non-standard and generic unnamed controls.
	/// </summary>
	public enum InputControlType : int
	{
		None,

		// Standardized controls.
		//
		LeftStickUp,
		LeftStickDown,
		LeftStickLeft,
		LeftStickRight,

		LeftStickButton,

		RightStickUp,
		RightStickDown,
		RightStickLeft,
		RightStickRight,

		RightStickButton,

		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,

		Action1,
		Action2,
		Action3,
		Action4,

		LeftTrigger,
		RightTrigger,

		LeftBumper,
		RightBumper,

		// This standardized control is an alias that maps onto all command buttons.
		//
		Command,

		// Command buttons.
		Back,
		Start,
		Select,
		System,
		Options,
		Pause,
		Menu,
		Share,
		Home,
		View,
		Power,

		// Unusual controls.
		//
		TiltX,
		TiltY,
		TiltZ,
		ScrollWheel,
		TouchPadTap,
		TouchPadXAxis,
		TouchPadYAxis,


		// Generic controls (usually assigned to unknown devices).
		//
		Analog0,
		Analog1,
		Analog2,
		Analog3,
		Analog4,
		Analog5,
		Analog6,
		Analog7,
		Analog8,
		Analog9,
		Analog10,
		Analog11,
		Analog12,
		Analog13,
		Analog14,
		Analog15,
		Analog16,
		Analog17,
		Analog18,
		Analog19,

		Button0,
		Button1,
		Button2,
		Button3,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8,
		Button9,
		Button10,
		Button11,
		Button12,
		Button13,
		Button14,
		Button15,
		Button16,
		Button17,
		Button18,
		Button19,


		// Internal. Must be last.
		//
		Count
	}
}

