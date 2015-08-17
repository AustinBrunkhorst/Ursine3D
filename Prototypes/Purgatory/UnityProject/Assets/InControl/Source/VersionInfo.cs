using System;
using System.Text.RegularExpressions;
using UnityEngine;


namespace InControl
{
	/// <summary>
	/// Encapsulates a comparable version number.
	/// This version number generally conforms to the semantic version system, 
	/// at least as far as InControl versioning is concerned.
	/// </summary>
	public struct VersionInfo : IComparable<VersionInfo>
	{
		/// <summary>
		/// The major version component.
		/// This number changes when significant incompatible API changes are made.
		/// </summary>
		public int Major;

		/// <summary>
		/// The minor version component.
		/// This number changes when significant functionality is added in a backwards-compatible manner.
		/// </summary>
		public int Minor;

		/// <summary>
		/// The patch version component.
		/// This number is changed when bug fixes are added in a backwards-compatible manner.
		/// </summary>
		public int Patch;

		/// <summary>
		/// The build version component.
		/// This number is incremented during development.
		/// </summary>
		public int Build;


		/// <summary>
		/// Initializes a new instance of the <see cref="InControl.VersionInfo"/> with 
		/// given version components.
		/// </summary>
		/// <param name="major">The major version component.</param>
		/// <param name="minor">The minor version component.</param>
		/// <param name="patch">The patch version component.</param>
		/// <param name="build">The build version component.</param>
		public VersionInfo( int major, int minor, int patch, int build )
		{
			Major = major;
			Minor = minor;
			Patch = patch;
			Build = build;
		}


		/// <summary>
		/// Initialize an instance of <see cref="InControl.VersionInfo"/> with
		/// the current version of InControl.
		/// </summary>
		/// <returns>The current version of InControl.</returns>
		public static VersionInfo InControlVersion()
		{
			return new VersionInfo() {
				Major = 1,
				Minor = 5,
				Patch = 9,
				Build = 5995
			};
		}


		/// <summary>
		/// Initialize an instance of <see cref="InControl.VersionInfo"/> with
		/// the current version of Unity.
		/// </summary>
		/// <returns>The current version of Unity.</returns>
		internal static VersionInfo UnityVersion()
		{
			var match = Regex.Match( Application.unityVersion, @"^(\d+)\.(\d+)\.(\d+)" );
			var build = 0;
			return new VersionInfo() {
				Major = Convert.ToInt32( match.Groups[1].Value ),
				Minor = Convert.ToInt32( match.Groups[2].Value ),
				Patch = Convert.ToInt32( match.Groups[3].Value ),
				Build = build
			};
		}


		/// <summary>
		/// Returns the sort order of the current instance compared to the specified object.
		/// </summary>
		public int CompareTo( VersionInfo other )
		{
			if (Major < other.Major) return -1;
			if (Major > other.Major) return +1;
			if (Minor < other.Minor) return -1;
			if (Minor > other.Minor) return +1;
			if (Patch < other.Patch) return -1;
			if (Patch > other.Patch) return +1;
			if (Build < other.Build) return -1;
			if (Build > other.Build) return +1;
			return 0;
		}


		/// <summary>
		/// Compares two instances of <see cref="InControl.VersionInfo"/> for equality.
		/// </summary>
		public static bool operator ==( VersionInfo a, VersionInfo b )
		{
			return a.CompareTo( b ) == 0;
		}


		/// <summary>
		/// Compares two instances of <see cref="InControl.VersionInfo"/> for inequality.
		/// </summary>
		public static bool operator !=( VersionInfo a, VersionInfo b )
		{
			return a.CompareTo( b ) != 0;
		}


		/// <summary>
		/// Compares two instances of <see cref="InControl.VersionInfo"/> to see if 
		/// the first is equal to or smaller than the second.
		/// </summary>
		public static bool operator <=( VersionInfo a, VersionInfo b )
		{
			return a.CompareTo( b ) <= 0;
		}


		/// <summary>
		/// Compares two instances of <see cref="InControl.VersionInfo"/> to see if 
		/// the first is equal to or larger than the second.
		/// </summary>
		public static bool operator >=( VersionInfo a, VersionInfo b )
		{
			return a.CompareTo( b ) >= 0;
		}


		/// <summary>
		/// Compares two instances of <see cref="InControl.VersionInfo"/> to see if 
		/// the first is smaller than the second.
		/// </summary>
		public static bool operator <( VersionInfo a, VersionInfo b )
		{
			return a.CompareTo( b ) < 0;
		}


		/// <summary>
		/// Compares two instances of <see cref="InControl.VersionInfo"/> to see if 
		/// the first is larger than the second.
		/// </summary>
		public static bool operator >( VersionInfo a, VersionInfo b )
		{
			return a.CompareTo( b ) > 0;
		}


		/// <summary>
		/// Determines whether the specified <see cref="System.Object"/> is equal to the current <see cref="InControl.VersionInfo"/>.
		/// </summary>
		/// <param name="other">The <see cref="System.Object"/> to compare with the current <see cref="InControl.VersionInfo"/>.</param>
		/// <returns><c>true</c> if the specified <see cref="System.Object"/> is equal to the current
		/// <see cref="InControl.VersionInfo"/>; otherwise, <c>false</c>.</returns>
		public override bool Equals( object other )
		{
			if (other is VersionInfo)
			{
				return this == ((VersionInfo) other);
			}
			return false;
		}


		/// <summary>
		/// Serves as a hash function for a <see cref="InControl.VersionInfo"/> object.
		/// </summary>
		/// <returns>A hash code for this instance that is suitable for use in hashing algorithms 
		/// and data structures such as a hash table.</returns>
		public override int GetHashCode()
		{
			return Major.GetHashCode() ^ Minor.GetHashCode() ^ Patch.GetHashCode() ^ Build.GetHashCode();
		}


		/// <summary>
		/// Returns a <see cref="System.String"/> that represents the current <see cref="InControl.VersionInfo"/>.
		/// </summary>
		/// <returns>A <see cref="System.String"/> that represents the current <see cref="InControl.VersionInfo"/>.</returns>
		public override string ToString()
		{
			if (Build == 0)
			{
				return string.Format( "{0}.{1}.{2}", Major, Minor, Patch );
			}
			return string.Format( "{0}.{1}.{2} build {3}", Major, Minor, Patch, Build );
		}


		/// <summary>
		/// Returns a shorter <see cref="System.String"/> that represents the current <see cref="InControl.VersionInfo"/>.
		/// </summary>
		/// <returns>A shorter <see cref="System.String"/> that represents the current <see cref="InControl.VersionInfo"/>.</returns>
		public string ToShortString()
		{
			if (Build == 0)
			{
				return string.Format( "{0}.{1}.{2}", Major, Minor, Patch );
			}
			return string.Format( "{0}.{1}.{2}b{3}", Major, Minor, Patch, Build );
		}
	}
}