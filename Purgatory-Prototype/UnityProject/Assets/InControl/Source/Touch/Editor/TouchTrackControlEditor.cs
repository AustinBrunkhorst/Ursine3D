#if UNITY_EDITOR
using System.IO;
using UnityEditor;
using UnityEngine;


namespace InControl
{
	[CustomEditor(typeof(TouchTrackControl))]
	public class TouchTrackControlEditor : TouchControlEditor
	{
		void OnEnable()
		{
			LoadHeaderImage( "Images/TouchTrackHeader.png" );
		}
	}
}
#endif