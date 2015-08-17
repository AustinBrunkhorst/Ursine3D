using System;
using System.Collections;
using System.IO;
using UnityEngine;

#if NETFX_CORE
using Windows.Storage;
using Windows.Storage.Streams;
using System.Threading.Tasks;
#endif


namespace InControl
{
	public static class Utility
	{
		public const float Epsilon = 1.0e-7f;


		private static Vector2[] circleVertexList = {
			new Vector2( +0.0000f, +1.0000f ),
			new Vector2( +0.2588f, +0.9659f ),
			new Vector2( +0.5000f, +0.8660f ),
			new Vector2( +0.7071f, +0.7071f ),
			new Vector2( +0.8660f, +0.5000f ),
			new Vector2( +0.9659f, +0.2588f ),
			new Vector2( +1.0000f, +0.0000f ),
			new Vector2( +0.9659f, -0.2588f ),
			new Vector2( +0.8660f, -0.5000f ),
			new Vector2( +0.7071f, -0.7071f ),
			new Vector2( +0.5000f, -0.8660f ),
			new Vector2( +0.2588f, -0.9659f ),
			new Vector2( +0.0000f, -1.0000f ),
			new Vector2( -0.2588f, -0.9659f ),
			new Vector2( -0.5000f, -0.8660f ),
			new Vector2( -0.7071f, -0.7071f ),
			new Vector2( -0.8660f, -0.5000f ),
			new Vector2( -0.9659f, -0.2588f ),
			new Vector2( -1.0000f, -0.0000f ),
			new Vector2( -0.9659f, +0.2588f ),
			new Vector2( -0.8660f, +0.5000f ),
			new Vector2( -0.7071f, +0.7071f ),
			new Vector2( -0.5000f, +0.8660f ),
			new Vector2( -0.2588f, +0.9659f ),
			new Vector2( +0.0000f, +1.0000f )
		};


		public static void DrawCircleGizmo( Vector2 center, float radius )
		{
			var p = (circleVertexList[0] * radius) + center;
			var c = circleVertexList.Length;
			for (int i = 1; i < c; i++)
			{
				Gizmos.DrawLine( p, p = (circleVertexList[i] * radius) + center );
			}
		}


		public static void DrawCircleGizmo( Vector2 center, float radius, Color color )
		{
			Gizmos.color = color;
			DrawCircleGizmo( center, radius );
		}


		public static void DrawOvalGizmo( Vector2 center, Vector2 size )
		{
			var r = size / 2.0f;
			var p = Vector2.Scale( circleVertexList[0], r ) + center;
			var c = circleVertexList.Length;
			for (int i = 1; i < c; i++)
			{
				Gizmos.DrawLine( p, p = Vector2.Scale( circleVertexList[i], r ) + center );
			}
		}


		public static void DrawOvalGizmo( Vector2 center, Vector2 size, Color color )
		{
			Gizmos.color = color;
			DrawOvalGizmo( center, size );
		}


		public static void DrawRectGizmo( Rect rect )
		{
			var p0 = new Vector3( rect.xMin, rect.yMin );
			var p1 = new Vector3( rect.xMax, rect.yMin );
			var p2 = new Vector3( rect.xMax, rect.yMax );
			var p3 = new Vector3( rect.xMin, rect.yMax );
			Gizmos.DrawLine( p0, p1 );
			Gizmos.DrawLine( p1, p2 );
			Gizmos.DrawLine( p2, p3 );
			Gizmos.DrawLine( p3, p0 );
		}


		public static void DrawRectGizmo( Rect rect, Color color )
		{
			Gizmos.color = color;
			DrawRectGizmo( rect );
		}


		public static void DrawRectGizmo( Vector2 center, Vector2 size )
		{
			var hw = size.x / 2.0f;
			var hh = size.y / 2.0f;
			var p0 = new Vector3( center.x - hw, center.y - hh );
			var p1 = new Vector3( center.x + hw, center.y - hh );
			var p2 = new Vector3( center.x + hw, center.y + hh );
			var p3 = new Vector3( center.x - hw, center.y + hh );
			Gizmos.DrawLine( p0, p1 );
			Gizmos.DrawLine( p1, p2 );
			Gizmos.DrawLine( p2, p3 );
			Gizmos.DrawLine( p3, p0 );
		}


		public static void DrawRectGizmo( Vector2 center, Vector2 size, Color color )
		{
			Gizmos.color = color;
			DrawRectGizmo( center, size );
		}


		public static bool GameObjectIsCulledOnCurrentCamera( GameObject gameObject )
		{
			return (Camera.current.cullingMask & (1 << gameObject.layer)) == 0;
		}


		public static Color MoveColorTowards( Color color0, Color color1, float maxDelta )
		{
			var r = Mathf.MoveTowards( color0.r, color1.r, maxDelta );
			var g = Mathf.MoveTowards( color0.g, color1.g, maxDelta );
			var b = Mathf.MoveTowards( color0.b, color1.b, maxDelta );
			var a = Mathf.MoveTowards( color0.a, color1.a, maxDelta );
			return new Color( r, g, b, a );
		}


		public static float ApplyDeadZone( float value, float lowerDeadZone, float upperDeadZone )
		{
			if (value < 0.0f)
			{
				if (value > -lowerDeadZone)
				{
					return 0.0f;
				}

				if (value < -upperDeadZone)
				{
					return -1.0f;
				}
					
				return (value + lowerDeadZone) / (upperDeadZone - lowerDeadZone);
			}
			else
			{
				if (value < lowerDeadZone)
				{
					return 0.0f;
				}

				if (value > upperDeadZone)
				{
					return 1.0f;
				}

				return (value - lowerDeadZone) / (upperDeadZone - lowerDeadZone);
			}
		}


		public static Vector2 ApplyCircularDeadZone( Vector2 v, float lowerDeadZone, float upperDeadZone )
		{
			var magnitude = Mathf.InverseLerp( lowerDeadZone, upperDeadZone, v.magnitude );
			return v.normalized * magnitude;
		}


		public static Vector2 ApplyCircularDeadZone( float x, float y, float lowerDeadZone, float upperDeadZone )
		{
			return ApplyCircularDeadZone( new Vector2( x, y ), lowerDeadZone, upperDeadZone );
		}


		public static float ApplySmoothing( float thisValue, float lastValue, float deltaTime, float sensitivity )
		{
			// 1.0f and above is instant (no smoothing).
			if (Utility.Approximately( sensitivity, 1.0f ))
			{
				return thisValue;
			}

			// Apply sensitivity (how quickly the value adapts to changes).
			var maxDelta = deltaTime * sensitivity * 100.0f;

			// Snap to zero when changing direction quickly.
			if (Mathf.Sign( lastValue ) != Mathf.Sign( thisValue ))
			{
				lastValue = 0.0f;
			}

			return Mathf.MoveTowards( lastValue, thisValue, maxDelta );
		}


		public static float ApplySnapping( float value, float threshold )
		{
			if (value < -threshold)
			{
				return -1.0f;
			}

			if (value > threshold)
			{
				return 1.0f;
			}

			return 0.0f;
		}


		internal static bool TargetIsButton( InputControlType target )
		{
			return (target >= InputControlType.Action1 && target <= InputControlType.Action4) || (target >= InputControlType.Button0 && target <= InputControlType.Button19);
		}


		internal static bool TargetIsStandard( InputControlType target )
		{
			return target >= InputControlType.LeftStickUp && target <= InputControlType.RightBumper;
		}


		#if NETFX_CORE
		public static async Task<string> Async_ReadFromFile( string path )
		{
			string name = Path.GetFileName( path );
			string folderPath = Path.GetDirectoryName( path );
			StorageFolder folder = await StorageFolder.GetFolderFromPathAsync( folderPath );
			StorageFile file = await folder.GetFileAsync( name );
			return await FileIO.ReadTextAsync( file );
		}

		public static async Task Async_WriteToFile( string path, string data )
		{
			string name = Path.GetFileName( path );
			string folderPath = Path.GetDirectoryName( path );
			StorageFolder folder = await StorageFolder.GetFolderFromPathAsync( folderPath );
			StorageFile file = await folder.CreateFileAsync( name, CreationCollisionOption.ReplaceExisting );
		    await FileIO.WriteTextAsync( file, data );
		}
		#endif


		public static string ReadFromFile( string path )
		{
			#if NETFX_CORE
			return Async_ReadFromFile( path ).Result;
			#else
			var streamReader = new StreamReader( path );
			var data = streamReader.ReadToEnd();
			streamReader.Close();
			return data;
			#endif
		}


		public static void WriteToFile( string path, string data )
		{
			#if NETFX_CORE
			Async_WriteToFile( path, data ).Wait();
			#else
			var streamWriter = new StreamWriter( path );
			streamWriter.Write( data );
			streamWriter.Flush();
			streamWriter.Close();
			#endif
		}


		public static float Abs( float value )
		{
			return value < 0.0f ? -value : value;
		}


		public static bool Approximately( float value1, float value2 )
		{
			var delta = value1 - value2;
			return (delta >= -Epsilon) && (delta <= Epsilon);
		}


		public static bool IsNotZero( float value )
		{
			return (value < -Epsilon) || (value > Epsilon);
		}


		public static bool IsZero( float value )
		{
			return (value >= -Epsilon) && (value <= Epsilon);
		}


		public static bool AbsoluteIsOverThreshold( float value, float threshold )
		{
			return (value < -threshold) || (value > threshold);
		}


		public static float NormalizeAngle( float angle )
		{
			while (angle < 0.0f)
			{
				angle += 360.0f;
			}

			while (angle > 360.0f)
			{
				angle -= 360.0f;
			}

			return angle;
		}


		public static float VectorToAngle( Vector2 vector )
		{
			if (Utility.IsZero( vector.x ) && Utility.IsZero( vector.y ))
			{
				return 0.0f;
			}
			return Utility.NormalizeAngle( Mathf.Atan2( vector.x, vector.y ) * Mathf.Rad2Deg );
		}


		public static float Min( float v0, float v1, float v2, float v3 )
		{
			var r0 = (v0 >= v1) ? v1 : v0;
			var r1 = (v2 >= v3) ? v3 : v2;
			return (r0 >= r1) ? r1 : r0;
		}


		public static float Max( float v0, float v1, float v2, float v3 )
		{
			var r0 = (v0 <= v1) ? v1 : v0;
			var r1 = (v2 <= v3) ? v3 : v2;
			return (r0 <= r1) ? r1 : r0;
		}


		internal static float ValueFromSides( float negativeSide, float positiveSide )
		{
			var nsv = Utility.Abs( negativeSide );
			var psv = Utility.Abs( positiveSide );

			if (Utility.Approximately( nsv, psv ))
			{
				return 0.0f;
			}

			return nsv > psv ? -nsv : psv;
		}


		internal static float ValueFromSides( float negativeSide, float positiveSide, bool invertSides )
		{
			if (invertSides)
			{
				return ValueFromSides( positiveSide, negativeSide );
			}
			else
			{
				return ValueFromSides( negativeSide, positiveSide );
			}
		}
	}
}



