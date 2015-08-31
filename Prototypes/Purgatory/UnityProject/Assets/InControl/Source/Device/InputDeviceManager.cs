using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace InControl
{
	public abstract class InputDeviceManager
	{
		protected List<InputDevice> devices = new List<InputDevice>();

		public abstract void Update( ulong updateTick, float deltaTime );


		public virtual void Destroy()
		{
		}
	}
}

