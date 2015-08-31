#if UNITY_EDITOR
using System;
using UnityEditor;
using UnityEngine;


namespace InControl
{
	public static class EditorUtility
	{
		public static T LoadAssetAtPath<T>( string assetPath )
			where T: UnityEngine.Object
		{
			#if UNITY_5 && !(UNITY_5_0_0 || UNITY_5_0_1)
			return AssetDatabase.LoadAssetAtPath<T>( assetPath );
			#else
			return Resources.LoadAssetAtPath<T>( assetPath );
			#endif
		}
	}
}
#endif