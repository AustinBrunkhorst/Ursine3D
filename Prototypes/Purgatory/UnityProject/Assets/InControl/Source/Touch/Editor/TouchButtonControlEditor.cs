#if UNITY_EDITOR
using System.IO;
using UnityEditor;
using UnityEngine;


namespace InControl
{
	[CustomEditor(typeof(TouchButtonControl))]
	public class TouchButtonControlEditor : TouchControlEditor
	{
		void OnEnable()
		{
			LoadHeaderImage( "Images/TouchButtonHeader.png" );
		}		
	}
}
#endif