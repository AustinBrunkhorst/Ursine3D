using System;
using UnityEngine;


namespace InControl
{
	public class MouseBindingSourceListener : BindingSourceListener
	{
		Mouse detectFound;
		int detectPhase;


		public void Reset()
		{
			detectFound = Mouse.None;
			detectPhase = 0; // Wait for release.
		}


		public BindingSource Listen( BindingListenOptions listenOptions, InputDevice device )
		{
			if (!listenOptions.IncludeMouseButtons)
			{
				return null;
			}

			if (detectFound != Mouse.None)
			{
				if (!MouseBindingSource.ButtonIsPressed( detectFound ))
				{
					if (detectPhase == 2)
					{
						var bindingSource = new MouseBindingSource( detectFound );
						Reset();
						return bindingSource;
					}
				}
			}

			var control = ListenForControl();
			if (control != Mouse.None)
			{
				if (detectPhase == 1)
				{
					detectFound = control;
					detectPhase = 2; // Wait for release.
				}
			}
			else
			{
				if (detectPhase == 0)
				{
					detectPhase = 1; // Wait for press.
				}
			}

			return null;
		}


		Mouse ListenForControl()
		{
			for (var control = Mouse.None; control <= Mouse.Button9; control++)
			{
				if (MouseBindingSource.ButtonIsPressed( control ))
				{
					return control;
				}
			}
			return Mouse.None;
		}
	}
}

