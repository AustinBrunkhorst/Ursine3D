using System;
using System.Collections;
using UnityEngine;
using InControl;


namespace KeyboardAndMouseExample
{
	// WARNING: "Custom profiles" will soon be deprecated in favor of the new user bindings API
	// (see the PlayerAction and PlayerActionSet classes) which accomplishes the same goal
	// much more elegantly and supports runtime remapping.
	// http://www.gallantgames.com/pages/incontrol-binding-actions-to-controls
	//
	// This custom profile is enabled by adding it to the Custom Profiles list
	// on the InControlManager component, or you can attach it yourself like so:
	// InputManager.AttachDevice( new UnityInputDevice( new KeyboardAndMouseProfile() ) );
	//
	public class KeyboardAndMouseProfile : CustomInputDeviceProfile
	{
		public KeyboardAndMouseProfile()
		{
			Name = "Keyboard/Mouse";
			Meta = "A keyboard and mouse combination profile appropriate for FPS.";

			ButtonMappings = new[] {
				new InputControlMapping {
					Handle = "Fire - Mouse",
					Target = InputControlType.Action1,
					Source = MouseButton0
				},
				new InputControlMapping {
					Handle = "Fire - Keyboard",
					Target = InputControlType.Action1,
					// KeyCodeButton fires when any of the provided KeyCode params are down.
					Source = KeyCodeButton( KeyCode.F, KeyCode.Return )
				},
				new InputControlMapping {
					Handle = "AltFire",
					Target = InputControlType.Action2,
					Source = MouseButton2
				},
				new InputControlMapping {
					Handle = "Middle",
					Target = InputControlType.Action3,
					Source = MouseButton1
				},
				new InputControlMapping {
					Handle = "Jump",
					Target = InputControlType.Action4,
					Source = KeyCodeButton( KeyCode.Space )
				},
				new InputControlMapping {
					Handle = "Combo",
					Target = InputControlType.LeftBumper,
					// KeyCodeComboButton requires that all KeyCode params are down simultaneously.
					Source = KeyCodeComboButton( KeyCode.LeftAlt, KeyCode.Alpha1 )
				},
			};

			AnalogMappings = new[] {
				new InputControlMapping {
					Handle = "Move Up",
					Target = InputControlType.LeftStickUp,
					Source = KeyCodeButton( KeyCode.W, KeyCode.UpArrow )
				},
				new InputControlMapping {
					Handle = "Move Down",
					Target = InputControlType.LeftStickDown,
					Source = KeyCodeButton( KeyCode.S, KeyCode.DownArrow )
				},
				new InputControlMapping {
					Handle = "Move Left",
					Target = InputControlType.LeftStickLeft,
					Source = KeyCodeButton( KeyCode.A, KeyCode.LeftArrow )
				},
				new InputControlMapping {
					Handle = "Move Right",
					Target = InputControlType.LeftStickRight,
					Source = KeyCodeButton( KeyCode.D, KeyCode.RightArrow )
				},
				new InputControlMapping {
					Handle = "Look Up",
					Target = InputControlType.RightStickUp,
					Source = MouseYAxis,
					SourceRange = InputRange.ZeroToPositiveInfinity,
					Raw = true,
					Scale = 0.1f
				},
				new InputControlMapping {
					Handle = "Look Down",
					Target = InputControlType.RightStickDown,
					Source = MouseYAxis,
					SourceRange = InputRange.ZeroToNegativeInfinity,
					Raw = true,
					Scale = 0.1f,
					Invert = true
				},
				new InputControlMapping {
					Handle = "Look Left",
					Target = InputControlType.RightStickLeft,
					Source = MouseXAxis,
					SourceRange = InputRange.ZeroToNegativeInfinity,
					Raw = true,
					Scale = 0.1f,
					Invert = true
				},
				new InputControlMapping {
					Handle = "Look Right",
					Target = InputControlType.RightStickRight,
					Source = MouseXAxis,
					SourceRange = InputRange.ZeroToPositiveInfinity,
					Raw = true,
					Scale = 0.1f
				},
//				new InputControlMapping {
//					Handle = "Look Z",
//					Target = InputControlType.ScrollWheel,
//					Source = MouseScrollWheel,
//					SourceRange = InputRange.Everything,
//					Raw = true,
//					Scale = 0.1f
//				},
				new InputControlMapping {
					Handle = "Zoom In",
					Target = InputControlType.RightTrigger,
					Source = MouseScrollWheel,
					SourceRange = InputRange.ZeroToNegativeInfinity,
					Invert = true,
					Raw = true,
					Scale = 1.0f
				},
				new InputControlMapping {
					Handle = "Zoom Out",
					Target = InputControlType.LeftTrigger,
					Source = MouseScrollWheel,
					SourceRange = InputRange.ZeroToPositiveInfinity,
					Raw = true,
					Scale = 1.0f
				}
			};
		}
	}
}

