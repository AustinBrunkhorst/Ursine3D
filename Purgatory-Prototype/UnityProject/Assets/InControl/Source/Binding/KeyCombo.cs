using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;


namespace InControl
{
	/// <summary>
	/// Represents a combination of one or more keys, including modifiers, up to a maximum of eight.
	/// </summary>
	public struct KeyCombo
	{
		int size;
		ulong data;


		public KeyCombo( params Key[] keys )
		{
			data = 0;
			size = 0;
			for (var i = 0; i < keys.Length; i++)
			{
				Add( keys[i] );
			}
		}


		void AddInt( int key )
		{
			if (size == 8)
			{
				return;
			}

			data = data | (((ulong) key & 0xFF) << (size * 8));
			size = size + 1;
		}


		int GetInt( int index )
		{
			return (int) ((data >> (index * 8)) & 0xFF);
		}


		public void Add( Key key )
		{
			AddInt( (int) key );
		}


		public Key Get( int index )
		{
			if (index < 0 || index >= size)
			{
				throw new IndexOutOfRangeException( "Index " + index + " is out of the range 0.." + size );
			}
			return (Key) GetInt( index );
		}


		public void Clear()
		{
			data = 0;
			size = 0;
		}


		public int Count
		{
			get
			{
				return size;
			}
		}


		public bool IsPressed
		{
			get
			{
				if (size == 0)
				{
					return false;
				}

				bool isPressed = true;
				for (var i = 0; i < size; i++)
				{
					var key = GetInt( i );
					isPressed = isPressed && KeyInfo.KeyList[key].IsPressed;
				}

				return isPressed;
			}
		}


		public static KeyCombo Detect( bool modifiersAsKeys )
		{
			var keyCombo = new KeyCombo();

			if (modifiersAsKeys)
			{
				for (var i = 1; i < 5; i++)
				{
					if (KeyInfo.KeyList[i].IsPressed)
					{
						keyCombo.AddInt( i );
					}
				}
			}
			else
			{
				for (var i = 5; i < 13; i++)
				{
					if (KeyInfo.KeyList[i].IsPressed)
					{
						keyCombo.AddInt( i );
						return keyCombo;
					}
				}
			}

			for (var i = 13; i < KeyInfo.KeyList.Length; i++)
			{
				if (KeyInfo.KeyList[i].IsPressed)
				{
					keyCombo.AddInt( i );
					return keyCombo;
				}
			}

			keyCombo.Clear();
			return keyCombo;
		}


		static Dictionary<ulong, string> cachedStrings = new Dictionary<ulong, string>();
		public override string ToString()
		{
			string value;
			if (!cachedStrings.TryGetValue( data, out value ))
			{
				value = "";
				for (var i = 0; i < size; i++)
				{
					if (i != 0)
					{
						value += " ";
					}
					var key = GetInt( i );
					value += KeyInfo.KeyList[key].Name;
				}
			}
			return value;
		}


		public static bool operator ==( KeyCombo a, KeyCombo b )
		{
			return a.data == b.data;
		}


		public static bool operator !=( KeyCombo a, KeyCombo b )
		{
			return a.data != b.data;
		}


		public override bool Equals( object other )
		{
			if (other is KeyCombo)
			{
				var keyCode = (KeyCombo) other;
				return data == keyCode.data;
			}

			return false;
		}


		public override int GetHashCode()
		{
			return data.GetHashCode();
		}


		internal void Load( BinaryReader reader )
		{
			size = reader.ReadInt32();
			data = reader.ReadUInt64();
		}


		internal void Save( BinaryWriter writer )
		{
			writer.Write( size );
			writer.Write( data );
		}
	}
}

