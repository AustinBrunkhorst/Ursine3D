using System;
using UnityEngine;


namespace InControl
{
	public class InputControl : InputControlBase
	{
		public static readonly InputControl Null = new InputControl();

		public string Handle { get; protected set; }
		public InputControlType Target { get; protected set; }

		public bool IsButton { get; protected set; }
		public bool IsAnalog { get; protected set; }

		ulong zeroTick;


		private InputControl() : base()
		{
			Handle = "None";
			Target = InputControlType.None;
		}


		public InputControl( string handle, InputControlType target ) : base()
		{
			Handle = handle;
			Target = target;

			IsButton = Utility.TargetIsButton( target );
			IsAnalog = !IsButton;
		}


		internal void SetZeroTick()
		{ 
			zeroTick = UpdateTick; 
		}


		internal bool IsOnZeroTick
		{
			get { return UpdateTick == zeroTick; }
		}


		public bool IsStandard
		{
			get
			{
				return Utility.TargetIsStandard( Target );
			}
		}
	}
}