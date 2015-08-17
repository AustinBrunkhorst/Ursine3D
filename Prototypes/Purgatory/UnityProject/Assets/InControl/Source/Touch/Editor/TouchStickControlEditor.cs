#if UNITY_EDITOR
using System.IO;
using UnityEditor;
using UnityEngine;


namespace InControl
{
	[CustomEditor( typeof(TouchStickControl) )]
	public class TouchStickControlEditor : TouchControlEditor
	{
		void OnEnable()
		{
			LoadHeaderImage( "Images/TouchStickHeader.png" );
		}
	}
}
#endif