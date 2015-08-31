using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace InControl
{
	public struct KeyInfo
	{
		private readonly Key key;
		private readonly string name;
		private readonly string macName;
		private readonly KeyCode[] keyCodes;


		private KeyInfo( Key key, string name, params KeyCode[] keyCodes )
		{
			this.key = key;
			this.name = name;
			this.macName = name;
			this.keyCodes = keyCodes;
		}


		private KeyInfo( Key key, string name, string macName, params KeyCode[] keyCodes )
		{
			this.key = key;
			this.name = name;
			this.macName = macName;
			this.keyCodes = keyCodes;
		}


		public bool IsPressed
		{
			get
			{
				var keyCodeCount = keyCodes.Length;
				for (int i = 0; i < keyCodeCount; i++)
				{
					if (Input.GetKey( keyCodes[i] ))
					{
						return true;
					}
				}
				return false;
			}
		}


		public string Name
		{
			get
			{
				if (Application.platform == RuntimePlatform.OSXEditor ||
				    Application.platform == RuntimePlatform.OSXPlayer ||
				    Application.platform == RuntimePlatform.OSXWebPlayer ||
				    Application.platform == RuntimePlatform.OSXDashboardPlayer)
				{
					return macName;
				}
				else
				{
					return name;
				}
			}
		}


		public Key Key
		{
			get
			{
				return key;
			}
		}


		public static readonly KeyInfo[] KeyList = {
			new KeyInfo( Key.None, "None", KeyCode.None ),

			new KeyInfo( Key.Shift, "Shift", KeyCode.LeftShift, KeyCode.RightShift ),
			new KeyInfo( Key.Alt, "Alt", "Option", KeyCode.LeftAlt, KeyCode.RightAlt ),
			new KeyInfo( Key.Command, "Command", KeyCode.LeftCommand, KeyCode.RightCommand ),
			new KeyInfo( Key.Control, "Control", KeyCode.LeftControl, KeyCode.RightControl ),

			new KeyInfo( Key.LeftShift, "Left Shift", KeyCode.LeftShift ),
			new KeyInfo( Key.LeftAlt, "Left Alt", "Left Option", KeyCode.LeftAlt ),
			new KeyInfo( Key.LeftCommand, "Left Command", KeyCode.LeftCommand ),
			new KeyInfo( Key.LeftControl, "Left Control", KeyCode.LeftControl ),

			new KeyInfo( Key.RightShift, "Right Shift", KeyCode.RightShift ),
			new KeyInfo( Key.RightAlt, "Right Alt", "Right Option", KeyCode.RightAlt ),
			new KeyInfo( Key.RightCommand, "Right Command", KeyCode.RightCommand ),
			new KeyInfo( Key.RightControl, "Right Control", KeyCode.RightControl ),

			new KeyInfo( Key.Escape, "Escape", KeyCode.Escape ),
			new KeyInfo( Key.F1, "F1", KeyCode.F1 ),
			new KeyInfo( Key.F2, "F2", KeyCode.F2 ),
			new KeyInfo( Key.F3, "F3", KeyCode.F3 ),
			new KeyInfo( Key.F4, "F4", KeyCode.F4 ),
			new KeyInfo( Key.F5, "F5", KeyCode.F5 ),
			new KeyInfo( Key.F6, "F6", KeyCode.F6 ),
			new KeyInfo( Key.F7, "F7", KeyCode.F7 ),
			new KeyInfo( Key.F8, "F8", KeyCode.F8 ),
			new KeyInfo( Key.F9, "F9", KeyCode.F9 ),
			new KeyInfo( Key.F10, "F10", KeyCode.F10 ),
			new KeyInfo( Key.F11, "F11", KeyCode.F11 ),
			new KeyInfo( Key.F12, "F12", KeyCode.F12 ),

			new KeyInfo( Key.Key0, "Num 0", KeyCode.Alpha0 ),
			new KeyInfo( Key.Key1, "Num 1", KeyCode.Alpha1 ),
			new KeyInfo( Key.Key2, "Num 2", KeyCode.Alpha2 ),
			new KeyInfo( Key.Key3, "Num 3", KeyCode.Alpha3 ),
			new KeyInfo( Key.Key4, "Num 4", KeyCode.Alpha4 ),
			new KeyInfo( Key.Key5, "Num 5", KeyCode.Alpha5 ),
			new KeyInfo( Key.Key6, "Num 6", KeyCode.Alpha6 ),
			new KeyInfo( Key.Key7, "Num 7", KeyCode.Alpha7 ),
			new KeyInfo( Key.Key8, "Num 8", KeyCode.Alpha8 ),
			new KeyInfo( Key.Key9, "Num 9", KeyCode.Alpha9 ),

			new KeyInfo( Key.A, "A", KeyCode.A ),
			new KeyInfo( Key.B, "B", KeyCode.B ),
			new KeyInfo( Key.C, "C", KeyCode.C ),
			new KeyInfo( Key.D, "D", KeyCode.D ),
			new KeyInfo( Key.E, "E", KeyCode.E ),
			new KeyInfo( Key.F, "F", KeyCode.F ),
			new KeyInfo( Key.G, "G", KeyCode.G ),
			new KeyInfo( Key.H, "H", KeyCode.H ),
			new KeyInfo( Key.I, "I", KeyCode.I ),
			new KeyInfo( Key.J, "J", KeyCode.J ),
			new KeyInfo( Key.K, "K", KeyCode.K ),
			new KeyInfo( Key.L, "L", KeyCode.L ),
			new KeyInfo( Key.M, "M", KeyCode.M ),
			new KeyInfo( Key.N, "N", KeyCode.N ),
			new KeyInfo( Key.O, "O", KeyCode.O ),
			new KeyInfo( Key.P, "P", KeyCode.P ),
			new KeyInfo( Key.Q, "Q", KeyCode.Q ),
			new KeyInfo( Key.R, "R", KeyCode.R ),
			new KeyInfo( Key.S, "S", KeyCode.S ),
			new KeyInfo( Key.T, "T", KeyCode.T ),
			new KeyInfo( Key.U, "U", KeyCode.U ),
			new KeyInfo( Key.V, "V", KeyCode.V ),
			new KeyInfo( Key.W, "W", KeyCode.W ),
			new KeyInfo( Key.X, "X", KeyCode.X ),
			new KeyInfo( Key.Y, "Y", KeyCode.Y ),
			new KeyInfo( Key.Z, "Z", KeyCode.Z ),

			new KeyInfo( Key.Backquote, "Backquote", KeyCode.BackQuote ),
			new KeyInfo( Key.Minus, "Minus", KeyCode.Minus ),
			new KeyInfo( Key.Equals, "Equals", KeyCode.Equals ),
			new KeyInfo( Key.Backspace, "Backspace", "Delete", KeyCode.Backspace ),

			new KeyInfo( Key.Tab, "Tab", KeyCode.Tab ),
			new KeyInfo( Key.LeftBracket, "Left Bracket", KeyCode.LeftBracket ),
			new KeyInfo( Key.RightBracket, "Right Bracket", KeyCode.RightBracket ),
			new KeyInfo( Key.Backslash, "Backslash", KeyCode.Backslash ),

			new KeyInfo( Key.Semicolon, "Semicolon", KeyCode.Semicolon ),
			new KeyInfo( Key.Quote, "Quote", KeyCode.Quote ),
			new KeyInfo( Key.Return, "Return", KeyCode.Return ),

			new KeyInfo( Key.Comma, "Comma", KeyCode.Comma ),
			new KeyInfo( Key.Period, "Period", KeyCode.Period ),
			new KeyInfo( Key.Slash, "Slash", KeyCode.Slash ),

			new KeyInfo( Key.Space, "Space", KeyCode.Space ),

			new KeyInfo( Key.Insert, "Insert", KeyCode.Insert ),
			new KeyInfo( Key.Delete, "Delete", "Forward Delete", KeyCode.Delete ),
			new KeyInfo( Key.Home, "Home", KeyCode.Home ),
			new KeyInfo( Key.End, "End", KeyCode.End ),
			new KeyInfo( Key.PageUp, "PageUp", KeyCode.PageUp ),
			new KeyInfo( Key.PageDown, "PageDown", KeyCode.PageDown ),

			new KeyInfo( Key.LeftArrow, "Left Arrow", KeyCode.LeftArrow ),
			new KeyInfo( Key.RightArrow, "Right Arrow", KeyCode.RightArrow ),
			new KeyInfo( Key.UpArrow, "Up Arrow", KeyCode.UpArrow ),
			new KeyInfo( Key.DownArrow, "Down Arrow", KeyCode.DownArrow ),

			new KeyInfo( Key.Pad0, "Pad 0", KeyCode.Keypad0 ),
			new KeyInfo( Key.Pad1, "Pad 1", KeyCode.Keypad1 ),
			new KeyInfo( Key.Pad2, "Pad 2", KeyCode.Keypad2 ),
			new KeyInfo( Key.Pad3, "Pad 3", KeyCode.Keypad3 ),
			new KeyInfo( Key.Pad4, "Pad 4", KeyCode.Keypad4 ),
			new KeyInfo( Key.Pad5, "Pad 5", KeyCode.Keypad5 ),
			new KeyInfo( Key.Pad6, "Pad 6", KeyCode.Keypad6 ),
			new KeyInfo( Key.Pad7, "Pad 7", KeyCode.Keypad7 ),
			new KeyInfo( Key.Pad8, "Pad 8", KeyCode.Keypad8 ),
			new KeyInfo( Key.Pad9, "Pad 9", KeyCode.Keypad9 ),

			new KeyInfo( Key.Numlock, "Numlock", KeyCode.Numlock ),
			new KeyInfo( Key.PadDivide, "Pad Divide", KeyCode.KeypadDivide ),
			new KeyInfo( Key.PadMultiply, "Pad Multiply", KeyCode.KeypadMultiply ),
			new KeyInfo( Key.PadMinus, "Pad Minus", KeyCode.KeypadMinus ),
			new KeyInfo( Key.PadPlus, "Pad Plus", KeyCode.KeypadPlus ),
			new KeyInfo( Key.PadEnter, "Pad Enter", KeyCode.KeypadEnter ),
			new KeyInfo( Key.PadPeriod, "Pad Period", KeyCode.KeypadPeriod ),

			// Mac only?
			new KeyInfo( Key.Clear, "Clear", KeyCode.Clear ),
			new KeyInfo( Key.PadEquals, "Pad Equals", KeyCode.KeypadEquals ),
			new KeyInfo( Key.F13, "F13", KeyCode.F13 ),
			new KeyInfo( Key.F14, "F14", KeyCode.F14 ),
			new KeyInfo( Key.F15, "F15", KeyCode.F15 ),

			// Unity doesn't define/support these :(
			// new KeyInfo( Key.F16, "F16", KeyCode.F16 ),
			// new KeyInfo( Key.F17, "F17", KeyCode.F17 ),
			// new KeyInfo( Key.F18, "F18", KeyCode.F18 ),
			// new KeyInfo( Key.F19, "F19", KeyCode.F19 ),
		};
	}
}
