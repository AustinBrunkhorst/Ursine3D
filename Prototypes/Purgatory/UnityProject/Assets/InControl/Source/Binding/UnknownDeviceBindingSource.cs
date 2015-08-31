using System;
using System.IO;
using UnityEngine;


namespace InControl
{

	public class UnknownDeviceBindingSource : BindingSource
	{
		public UnknownDeviceControl Control { get; protected set; }


		internal UnknownDeviceBindingSource()
		{
			Control = UnknownDeviceControl.None;
		}


		public UnknownDeviceBindingSource( UnknownDeviceControl control )
		{
			Control = control;
		}


		public override float GetValue( InputDevice device )
		{
			return Control.GetValue( device );
		}


		public override bool GetState( InputDevice device )
		{
			if (device == null)
			{
				return false;
			}

			return Utility.IsNotZero( GetValue( device ) );
		}


		public override string Name
		{ 
			get
			{
				if (BoundTo == null)
				{
					Debug.LogError( "Cannot query property 'Name' for unbound BindingSource." );
					return null;
				}
				else
				{
					var prefix = "";
					if (Control.SourceRange == InputRangeType.ZeroToMinusOne)
					{
						prefix = "Negative ";
					}
					else
					if (Control.SourceRange == InputRangeType.ZeroToOne)
					{
						prefix = "Positive ";
					}

					var device = BoundTo.Device;
					if (device == InputDevice.Null)
					{
						return prefix + Control.Control.ToString();
					}

					var control = device.GetControl( Control.Control );
					if (control == InputControl.Null)
					{
						return prefix + Control.Control.ToString();
					}

					return prefix + control.Handle;
				}
			}
		}


		public override string DeviceName
		{ 
			get
			{
				if (BoundTo == null)
				{
					Debug.LogError( "Cannot query property 'DeviceName' for unbound BindingSource." );
					return null;
				}
				else
				{
					var device = BoundTo.Device;
					if (device == InputDevice.Null)
					{
						return "Unknown Controller";
					}

					return device.Name;
				}
			}
		}


		public override bool Equals( BindingSource other )
		{
			if (other == null)
			{
				return false;
			}

			var bindingSource = other as UnknownDeviceBindingSource;
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

			var bindingSource = other as UnknownDeviceBindingSource;
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
				return BindingSourceType.UnknownDeviceBindingSource;
			}
		}


		internal override bool IsValid
		{ 
			get
			{
				if (BoundTo == null)
				{
					Debug.LogError( "Cannot query property 'IsValid' for unbound BindingSource." );
					return false;
				}
				else
				{
					var device = BoundTo.Device;
					return device == InputDevice.Null || device.HasControl( Control.Control );
				}
			}
		}


		internal override void Load( BinaryReader reader )
		{
			// Have to do this because it's a struct property? Weird.
			var temp = new UnknownDeviceControl();
			temp.Load( reader );
			Control = temp;
		}


		internal override void Save( BinaryWriter writer )
		{
			Control.Save( writer );	
		}
	}
}


