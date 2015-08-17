#if UNITY_EDITOR
using System.IO;
using UnityEditor;
using UnityEngine;


namespace InControl
{
	[CustomEditor(typeof(TouchSwipeControl))]
	public class TouchSwipeControlEditor : TouchControlEditor
	{
		void OnEnable()
		{
			LoadHeaderImage( "Images/TouchSwipeHeader.png" );
		}
	}
}
#endif