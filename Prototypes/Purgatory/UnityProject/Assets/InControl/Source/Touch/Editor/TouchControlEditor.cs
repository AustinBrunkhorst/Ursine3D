#if UNITY_EDITOR
using System.IO;
using UnityEditor;
using UnityEngine;


namespace InControl
{
	public class TouchControlEditor : Editor
	{
		protected Texture headerTexture;
		Rect headerTextureRect;


		protected void LoadHeaderImage( string fileName )
		{
			var path = AssetDatabase.GetAssetPath( MonoScript.FromScriptableObject( this ) );
			headerTexture = EditorUtility.LoadAssetAtPath<Texture>( Path.GetDirectoryName( path ) + "/" + fileName );
		}


		protected void AddHeaderImageSpace()
		{
			if (headerTexture != null)
			{
				GUILayout.Space( 5 );

				headerTextureRect = GUILayoutUtility.GetRect( 0.0f, -22.0f );
				headerTextureRect.width = headerTexture.width;
				headerTextureRect.height = headerTexture.height;

				GUILayout.Space( headerTextureRect.height );
			}
		}


		protected void DrawHeaderImage()
		{
			if (headerTexture != null)
			{
				GUI.DrawTexture( headerTextureRect, headerTexture );
			}
		}


		public override void OnInspectorGUI()
		{
			AddHeaderImageSpace();

			if (DrawDefaultInspector())
			{
				if (Application.isPlaying)
				{
					foreach (var target in targets)
					{
						(target as TouchControl).SendMessage( "ConfigureControl" );
					}
				}
			}

			DrawHeaderImage();
		}
	}
}
#endif