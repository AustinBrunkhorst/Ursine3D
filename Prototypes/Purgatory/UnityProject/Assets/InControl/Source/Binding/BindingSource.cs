using System;
using System.IO;
using UnityEngine;


namespace InControl
{
	/// <summary>
	/// The abstract base class for all binding sources.
	/// A binding source can be bound to an action and provides an input source. It essentially
	/// represents a control bound to an action, whether it be a controller button, a key or combination
	/// of keys, or a mouse button, etc. An action may have multiple binding sources bound.
	/// An InputDevice may serve as context for a binding source, especially in the case of controllers.
	/// For example, the binding source may be "Left Trigger," but when querying a value for the 
	/// binding, a specific InputDevice must be provided. Not all bindings require an input
	/// device. Keyboard or mouse bindings do not.
	/// </summary>
	public abstract class BindingSource : InputControlSource, IEquatable<BindingSource>
	{
		/// <summary>
		/// Read a float value from the binding source in the context of an optional InputDevice.
		/// </summary>
		/// <returns>The value, usually in the range -1..1, but not necessarily, for example, 
		/// in the case of mouse movement.</returns>
		/// <param name="inputDevice">An input device which serves as the context for this source, if applicable. Pass in null when not applicable.</param>
		public abstract float GetValue( InputDevice inputDevice );


		/// <summary>
		/// Read a bool value from the binding source in the context of an optional InputDevice.
		/// </summary>
		/// <returns><c>true</c> if the value of the binding is non-zero; otherwise <c>false</c>.</returns>
		/// <param name="inputDevice">An input device which serves as the context for this source, if applicable. Pass in null when not applicable.</param>
		public abstract bool GetState( InputDevice inputDevice );


		/// <summary>
		/// Determines whether the specified BindingSource is equal to the current BindingSource.
		/// </summary>
		/// <param name="other">The BindingSource to compare with the current BindingSource.</param>
		/// <returns><c>true</c> if the specified BindingSource is equal to the current
		/// BindingSource; otherwise, <c>false</c>.</returns>
		public abstract bool Equals( BindingSource other );


		/// <summary>
		/// Gets a textual representation of the binding source.
		/// </summary>
		/// <value>The name.</value>
		public abstract string Name { get; }


		/// <summary>
		/// Gets the name of the device this binding source currently represents.
		/// </summary>
		/// <value>The name of the device.</value>
		public abstract string DeviceName { get; }


		/// <summary>
		/// Determines whether the specified binding sources are equal.
		/// </summary>
		/// <param name="a">The first binding source to compare.</param>
		/// <param name="b">The second binding source to compare.</param>
		/// <returns><c>true</c> if the specified binding sources are equal; otherwise, <c>false</c>.</returns>
		public static bool operator ==( BindingSource a, BindingSource b )
		{
			if (object.ReferenceEquals( a, b ))
			{
				return true;
			}

			if (((object) a == null) || ((object) b == null))
			{
				return false;
			}

			if (a.BindingSourceType != b.BindingSourceType)
			{
				return false;
			}

			return a.Equals( b );
		}


		/// <summary>
		/// Determines whether the specified binding sources are not equal.
		/// </summary>
		/// <param name="a">The first binding source to compare.</param>
		/// <param name="b">The second binding source to compare.</param>
		/// <returns><c>true</c> if the specified binding sources are not equal; otherwise, <c>false</c>.</returns>
		public static bool operator !=( BindingSource a, BindingSource b )
		{
			return !(a == b);
		}


		/// <summary>
		/// Determines whether the specified object is equal to the current BindingSource.
		/// </summary>
		/// <param name="other">The object to compare with the current BindingSource.</param>
		/// <returns><c>true</c> if the specified object is equal to the current
		/// BindingSource; otherwise, <c>false</c>.</returns>
		public override bool Equals( object other )
		{
			return Equals( (BindingSource) other );
		}


		/// <summary>
		/// Serves as a hash function for a BindingSource object.
		/// </summary>
		/// <returns>A hash code for this instance that is suitable for use in 
		/// hashing algorithms and data structures such as a hash table.</returns>
		public override int GetHashCode()
		{
			return base.GetHashCode();
		}


		#region Internal

		internal abstract BindingSourceType BindingSourceType { get; }
		internal abstract void Save( BinaryWriter writer );
		internal abstract void Load( BinaryReader reader );
		internal PlayerAction BoundTo { get; set; }


		internal virtual bool IsValid
		{
			get
			{
				return true;
			}
		}

		#endregion
	}
}

