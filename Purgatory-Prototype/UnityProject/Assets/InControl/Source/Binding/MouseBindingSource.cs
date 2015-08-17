using System;
using System.IO;
using UnityEngine;


namespace InControl
{
	public class MouseBindingSource : BindingSource
	{
		public Mouse Control { get; protected set; }


		internal MouseBindingSource()
		{
		}


		public MouseBindingSource( Mouse mouseControl )
		{
			Control = mouseControl;
		}


		// This is necessary to maintain backward compatibility. :(
		readonly static int[] buttonTable = new [] {
			-1, 0, 1, 2, -1, -1, -1, -1, -1, -1, 3, 4, 5, 6, 7, 8, 9
		};

		internal static bool ButtonIsPressed( Mouse control )
		{
			var button = buttonTable[(int) control];
			if (button >= 0)
			{
				return Input.GetMouseButton( button );
			}
			return false;
		}


		public override float GetValue( InputDevice inputDevice )
		{
			const float scale = 0.2f;

			var button = buttonTable[(int) Control];
			if (button >= 0)
			{
				return Input.GetMouseButton( button ) ? 1.0f : 0.0f;
			}

			switch (Control)
			{
				case Mouse.NegativeX:
					return -Mathf.Min( Input.GetAxisRaw( "mouse x" ) * scale, 0.0f );
				case Mouse.PositiveX:
					return Mathf.Max( 0.0f, Input.GetAxisRaw( "mouse x" ) * scale );
				case Mouse.NegativeY:
					return -Mathf.Min( Input.GetAxisRaw( "mouse y" ) * scale, 0.0f );
				case Mouse.PositiveY:
					return Mathf.Max( 0.0f, Input.GetAxisRaw( "mouse y" ) * scale );
				case Mouse.NegativeScrollWheel:
					return -Mathf.Min( Input.GetAxisRaw( "mouse z" ) * scale, 0.0f );
				case Mouse.PositiveScrollWheel:
					return Mathf.Max( 0.0f, Input.GetAxisRaw( "mouse z" ) * scale );
			}

			return 0.0f;
		}


		public override bool GetState( InputDevice inputDevice )
		{
			return Utility.IsNotZero( GetValue( inputDevice ) );
		}


		public override string Name
		{ 
			get
			{
				return Control.ToString();
			}
		}


		public override string DeviceName
		{ 
			get
			{
				return "Mouse";
			}
		}


		public override bool Equals( BindingSource other )
		{
			if (other == null)
			{
				return false;
			}

			var bindingSource = other as MouseBindingSource;
			if (bindingSource != null)
			{
				return Control == bindingSource.Control;
			}

			return false;
		}


		public override bool Equals( object other )
		{
			if (other == null)
			{
				return false;
			}

			var bindingSource = other as MouseBindingSource;
			if (bindingSource != null)
			{
				return Control == bindingSource.Control;
			}

			return false;
		}


		public override int GetHashCode()
		{
			return Control.GetHashCode();
		}


		internal override BindingSourceType BindingSourceType
		{
			get
			{
				return BindingSourceType.MouseBindingSource;
			}
		}


		internal override void Save( BinaryWriter writer )
		{
			writer.Write( (int) Control );
		}


		internal override void Load( BinaryReader reader )
		{
			Control = (Mouse) reader.ReadInt32();
		}
	}
}

