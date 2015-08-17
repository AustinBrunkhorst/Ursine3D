using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using UnityEngine;


namespace InControl
{
	/// <summary>
	/// This class represents a single action that may have multiple controls bound to it.
	/// A bound control is represented by a subclass of BindingSource. For example,
	/// DeviceBindingSource provides input from a control on any supported InputDevice.
	/// Similarly, KeyBindingSource provides input from one or more keypresses. An action
	/// may have any number of bindings.
	/// Actions have two groups of bindings defined: default bindings and regular bindings.
	/// Default bindings are the predefined default bindings, and the current bindings for
	/// the action can be reset to this group in a single operation. Regular bindings are those
	/// added by users, most likely at runtime in a settings menu or the like. There are no
	/// other distinctions between these groupings; they are purely for organizational convenience.
	/// </summary>
	public class PlayerAction : InputControlBase
	{
		/// <summary>
		/// The unique identifier for this action within the context of its owning action set.
		/// </summary>
		public string Name { get; private set; }

		/// <summary>
		/// Gets the owning action set containing this action.
		/// </summary>
		public PlayerActionSet Owner { get; private set; }

		/// <summary>
		/// Configures how this action listens for new bindings.
		/// When <c>null</c> (default) the owner's <see cref="PlayerActionSet.ListenOptions"/> will be used.
		/// <seealso cref="PlayerAction.ListenForBinding()"/>
		/// </summary>
		public BindingListenOptions ListenOptions = null;

		/// <summary>
		/// The binding source type that provided input to this action.
		/// </summary>
		public BindingSourceType LastInputType = BindingSourceType.None;

		List<BindingSource> defaultBindings = new List<BindingSource>();
		List<BindingSource> regularBindings = new List<BindingSource>();
		List<BindingSource> visibleBindings = new List<BindingSource>();

		readonly ReadOnlyCollection<BindingSource> bindings;

		readonly static BindingSourceListener[] bindingSourceListeners = new BindingSourceListener[] {
			new DeviceBindingSourceListener(),
			new UnknownDeviceBindingSourceListener(),
			new KeyBindingSourceListener(),
			new MouseBindingSourceListener()
		};


		/// <summary>
		/// Construct an action belonging to a given action set.
		/// </summary>
		/// <param name="name">A unique identifier for this action within the context of its owning action set.</param>
		/// <param name="owner">The action set to contain (own) this action.</param>
		public PlayerAction( string name, PlayerActionSet owner )
		{
			Raw = true;
			Name = name;
			Owner = owner;
			bindings = new ReadOnlyCollection<BindingSource>( visibleBindings );
		}


		/// <summary>
		/// Adds a default binding for the action. This will also add it to the regular bindings.
		/// </summary>
		/// <param name="binding">The BindingSource to add.</param>
		public void AddDefaultBinding( BindingSource binding )
		{
			if (binding == null)
			{
				return;
			}

			if (binding.BoundTo != null)
			{
				throw new InControlException( "Binding source is already bound to action " + binding.BoundTo.Name );
			}

			if (!defaultBindings.Contains( binding ))
			{
				defaultBindings.Add( binding );
				binding.BoundTo = this;
			}

			if (!regularBindings.Contains( binding ))
			{
				regularBindings.Add( binding );
				binding.BoundTo = this;

				if (binding.IsValid)
				{
					visibleBindings.Add( binding );
				}
			}
		}


		/// <summary>
		/// A convenience method for adding a KeyBindingSource to the default bindings.
		/// </summary>
		/// <param name="keys">A list of one or more keys making up a KeyCombo for the binding source.</param>
		public void AddDefaultBinding( params Key[] keys )
		{
			AddDefaultBinding( new KeyBindingSource( keys ) );
		}


		/// <summary>
		/// A convenience method for adding a MouseBindingSource to the default bindings.
		/// </summary>
		/// <param name="control">The Mouse control to add.</param>
		public void AddDefaultBinding( Mouse control )
		{
			AddDefaultBinding( new MouseBindingSource( control ) );
		}


		/// <summary>
		/// A convenience method for adding a DeviceBindingSource to the default bindings.
		/// </summary>
		/// <param name="control">The InputControlType to add.</param>
		public void AddDefaultBinding( InputControlType control )
		{
			AddDefaultBinding( new DeviceBindingSource( control ) );
		}


		/// <summary>
		/// Add a regular binding to the action. A binding cannot be added if it matches an
		/// existing binding on the action, or if it is already bound to another action.
		/// </summary>
		/// <returns><c>true</c>, if binding was added, <c>false</c> otherwise.</returns>
		/// <param name="binding">The BindingSource to add.</param>
		public bool AddBinding( BindingSource binding )
		{
			if (binding == null)
			{
				return false;
			}

			if (binding.BoundTo != null)
			{
				throw new InControlException( "Binding source is already bound to action " + binding.BoundTo.Name );
			}

			if (regularBindings.Contains( binding ))
			{
				return false;
			}

			regularBindings.Add( binding );
			binding.BoundTo = this;

			if (binding.IsValid)
			{
				visibleBindings.Add( binding );
			}

			return true;
		}


		internal bool HasBinding( BindingSource binding )
		{
			if (binding == null)
			{
				return false;
			}

			var foundBinding = FindBinding( binding );
			if (foundBinding == null)
			{
				return false;
			}

			return foundBinding.BoundTo == this;
		}


		internal BindingSource FindBinding( BindingSource binding )
		{
			if (binding == null)
			{
				return null;
			}

			var index = regularBindings.IndexOf( binding );
			if (index >= 0)
			{
				return regularBindings[index];
			}

			return null;
		}


		internal void FindAndRemoveBinding( BindingSource binding )
		{
			if (binding == null)
			{
				return;
			}

			var bindingIndex = regularBindings.IndexOf( binding );
			if (bindingIndex >= 0)
			{
				var foundBinding = regularBindings[bindingIndex];
				if (foundBinding.BoundTo == this)
				{
					foundBinding.BoundTo = null;
					regularBindings.RemoveAt( bindingIndex );
					UpdateVisibleBindings();
				}
			}
		}


		internal int CountBindingsOfType( BindingSourceType bindingSourceType )
		{
			int count = 0;
			var bindingCount = regularBindings.Count;
			for (int i = 0; i < bindingCount; i++)
			{
				var binding = regularBindings[i];
				if (binding.BoundTo == this && binding.BindingSourceType == bindingSourceType)
				{
					count += 1;
				}
			}
			return count;
		}


		internal void RemoveFirstBindingOfType( BindingSourceType bindingSourceType )
		{
			var bindingCount = regularBindings.Count;
			for (int i = 0; i < bindingCount; i++)
			{
				var binding = regularBindings[i];
				if (binding.BoundTo == this && binding.BindingSourceType == bindingSourceType)
				{
					binding.BoundTo = null;
					regularBindings.RemoveAt( i );
					return;
				}
			}
		}


		internal int IndexOfFirstInvalidBinding()
		{
			var bindingCount = regularBindings.Count;
			for (int i = 0; i < bindingCount; i++)
			{
				if (!regularBindings[i].IsValid)
				{
					return i;
				}
			}

			return -1;
		}


		/// <summary>
		/// Removes the binding from the action.
		/// Note: the action is only marked for removal, and is not immediately removed. This is
		/// to allow for safe removal during iteration over the Bindings collection.
		/// </summary>
		/// <param name="binding">The BindingSource to remove.</param>
		public void RemoveBinding( BindingSource binding )
		{
			if (binding == null)
			{
				return;
			}

			if (binding.BoundTo != this)
			{
				throw new InControlException( "Cannot remove a binding source not bound to this action." );
			}

			binding.BoundTo = null;
		}


		/// <summary>
		/// Clears the bindings for this action.
		/// </summary>
		public void ClearBindings()
		{
			var bindingCount = regularBindings.Count;
			for (int i = 0; i < bindingCount; i++)
			{
				regularBindings[i].BoundTo = null;
			}
			regularBindings.Clear();
			visibleBindings.Clear();
		}


		/// <summary>
		/// Resets the bindings to the default bindings.
		/// </summary>
		public void ResetBindings()
		{
			ClearBindings();

			regularBindings.AddRange( defaultBindings );

			var bindingCount = regularBindings.Count;
			for (int i = 0; i < bindingCount; i++)
			{
				var binding = regularBindings[i];

				binding.BoundTo = this;

				if (binding.IsValid)
				{
					visibleBindings.Add( binding );
				}
			}
		}


		/// <summary>
		/// Begin listening for a new user defined binding. Which types of BindingSource are detected
		/// depends on the value of ListenOptions and DefaultListenOptions. Once one is found, it will
		/// be added to the regular bindings for the action and listening will stop.
		/// </summary>
		public void ListenForBinding()
		{
			Owner.listenWithAction = this;

			var bindingSourceListenerCount = bindingSourceListeners.Length;
			for (int i = 0; i < bindingSourceListenerCount; i++)
			{
				bindingSourceListeners[i].Reset();
			}
		}


		/// <summary>
		/// Stop listening for new user defined bindings.
		/// </summary>
		public void StopListeningForBinding()
		{
			if (IsListeningForBinding)
			{
				Owner.listenWithAction = null;
			}
		}


		/// <summary>
		/// Gets a value indicating whether this action is listening for new user defined bindings.
		/// </summary>
		public bool IsListeningForBinding
		{
			get
			{
				return Owner.listenWithAction == this;
			}
		}


		/// <summary>
		/// Gets the current bindings for this action as a readonly collection.
		/// </summary>
		public ReadOnlyCollection<BindingSource> Bindings
		{
			get
			{
				return bindings;
			}
		}


		void RemoveOrphanedBindings()
		{
			var bindingCount = regularBindings.Count;
			for (int i = bindingCount - 1; i >= 0; i--)
			{
				if (regularBindings[i].BoundTo != this)
				{
					regularBindings.RemoveAt( i );
				}
			}
		}


		internal void Update( ulong updateTick, float deltaTime, InputDevice device )
		{
			Device = device;
			UpdateBindings( updateTick, deltaTime );
			DetectBindings();
		}


		void UpdateBindings( ulong updateTick, float deltaTime )
		{
			if (Owner.Enabled)
			{
				var bindingCount = regularBindings.Count;
				for (int i = bindingCount - 1; i >= 0; i--)
				{
					var binding = regularBindings[i];
					if (binding.BoundTo != this)
					{
						regularBindings.RemoveAt( i );
						visibleBindings.Remove( binding );
					}
					else
					{
						var value = binding.GetValue( Device );
						if (UpdateWithValue( value, updateTick, deltaTime ))
						{
							LastInputType = binding.BindingSourceType;
						}
					}
				}
			}
			else
			{
				UpdateWithValue( 0.0f, updateTick, deltaTime );
			}

			Commit();
		}


		void DetectBindings()
		{
			if (IsListeningForBinding)
			{
				BindingSource binding = null;
				var listenOptions = ListenOptions ?? Owner.ListenOptions;

				var bindingSourceListenerCount = bindingSourceListeners.Length;
				for (int i = 0; i < bindingSourceListenerCount; i++)
				{
					binding = bindingSourceListeners[i].Listen( listenOptions, device );
					if (binding != null)
					{
						break;
					}
				}

				if (binding == null)
				{
					// No binding found.
					return;
				}

				var onBindingFound = listenOptions.OnBindingFound;
				if (onBindingFound != null && !onBindingFound( this, binding ))
				{
					// Binding rejected by user.
					return;
				}

				if (HasBinding( binding ))
				{
					var onBindingRejected = listenOptions.OnBindingRejected;
					if (onBindingRejected != null)
					{
						onBindingRejected( this, binding, BindingSourceRejectionType.DuplicateBindingOnAction );
					}
					return;
				}

				if (listenOptions.UnsetDuplicateBindingsOnSet)
				{
					Owner.RemoveBinding( binding );
				}

				if (!listenOptions.AllowDuplicateBindingsPerSet && Owner.HasBinding( binding ))
				{					
					var onBindingRejected = listenOptions.OnBindingRejected;
					if (onBindingRejected != null)
					{
						onBindingRejected( this, binding, BindingSourceRejectionType.DuplicateBindingOnActionSet );
					}
					return;
				}

				StopListeningForBinding();

				if (listenOptions.MaxAllowedBindingsPerType > 0)
				{
					while (CountBindingsOfType( binding.BindingSourceType ) >= listenOptions.MaxAllowedBindingsPerType)
					{
						RemoveFirstBindingOfType( binding.BindingSourceType );
					}
				}
				else
				{
					if (listenOptions.MaxAllowedBindings > 0)
					{
						while (regularBindings.Count >= listenOptions.MaxAllowedBindings)
						{
							var removeIndex = Mathf.Max( 0, IndexOfFirstInvalidBinding() );
							regularBindings.RemoveAt( removeIndex );
						}
					}
				}

				AddBinding( binding );
				UpdateVisibleBindings();

				var onBindingAdded = listenOptions.OnBindingAdded;
				if (onBindingAdded != null)
				{
					onBindingAdded( this, binding );
				}
			}
		}


		void UpdateVisibleBindings()
		{
			visibleBindings.Clear();
			var bindingCount = regularBindings.Count;
			for (int i = 0; i < bindingCount; i++)
			{
				var binding = regularBindings[i];
				if (binding.IsValid)
				{
					visibleBindings.Add( binding );
				}
			}
		}


		InputDevice device;
		internal InputDevice Device
		{
			get
			{
				if (device == null)
				{
					device = Owner.Device;
					UpdateVisibleBindings();
				}

				return device;
			}

			set
			{
				if (device != value)
				{
					device = value;
					UpdateVisibleBindings();
				}
			}
		}



		internal void Load( BinaryReader reader )
		{
			ClearBindings();

			var bindingCount = reader.ReadInt32();
			for (int i = 0; i < bindingCount; i++)
			{
				var bindingSourceType = (BindingSourceType) reader.ReadInt32();
				BindingSource bindingSource;

				switch (bindingSourceType)
				{
					case BindingSourceType.DeviceBindingSource:
						bindingSource = new DeviceBindingSource();
						break;

					case BindingSourceType.KeyBindingSource:
						bindingSource = new KeyBindingSource();
						break;

					case BindingSourceType.MouseBindingSource:
						bindingSource = new MouseBindingSource();
						break;
					
					case BindingSourceType.UnknownDeviceBindingSource:
						bindingSource = new UnknownDeviceBindingSource();
						break;

					default:
						throw new InControlException( "Don't know how to load BindingSourceType: " + bindingSourceType );
				}

				bindingSource.Load( reader );
				AddBinding( bindingSource );
			}
		}


		internal void Save( BinaryWriter writer )
		{
			RemoveOrphanedBindings();

			writer.Write( Name );

			var bindingCount = regularBindings.Count;
			writer.Write( bindingCount );

			for (int i = 0; i < bindingCount; i++)
			{
				var binding = regularBindings[i];
				writer.Write( (int) binding.BindingSourceType );
				binding.Save( writer );
			}
		}
	}
}

