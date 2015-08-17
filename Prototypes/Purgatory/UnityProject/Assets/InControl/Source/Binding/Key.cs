using UnityEngine;
using System.Collections;


namespace InControl
{
	public enum Key : int
	{
		None,

		// Modifiers
		Shift,
		Alt,
		Command,
		Control,

		// Modifier keys as first class keys
		LeftShift,
		LeftAlt,
		LeftCommand,
		LeftControl,
		RightShift,
		RightAlt,
		RightCommand,
		RightControl,

		Escape,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		Key0,
		Key1,
		Key2,
		Key3,
		Key4,
		Key5,
		Key6,
		Key7,
		Key8,
		Key9,

		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		Backquote,
		Minus,
		Equals,
		Backspace,

		Tab,
		LeftBracket,
		RightBracket,
		Backslash,

		Semicolon,
		Quote,
		Return,

		Comma,
		Period,
		Slash,

		Space,

		Insert,
		Delete,
		Home,
		End,
		PageUp,
		PageDown,

		LeftArrow,
		RightArrow,
		UpArrow,
		DownArrow,

		Pad0,
		Pad1,
		Pad2,
		Pad3,
		Pad4,
		Pad5,
		Pad6,
		Pad7,
		Pad8,
		Pad9,

		Numlock,
		PadDivide,
		PadMultiply,
		PadMinus,
		PadPlus,
		PadEnter,
		PadPeriod,

		// Mac only?
		Clear,
		PadEquals,
		F13,
		F14,
		F15,

		// Unity doesn't define/support these. :(
		// F16,
		// F17,
		// F18,
		// F19,
	}
}


