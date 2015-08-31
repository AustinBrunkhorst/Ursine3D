using System;
using System.Reflection;
using UnityEngine;


namespace InControl
{
	[AttributeUsage( AttributeTargets.Class, Inherited = true )]
	public class SingletonPrefabAttribute : Attribute
	{
		public readonly string Name;

		public SingletonPrefabAttribute( string name )
		{
			Name = name;
		}
	}


	public abstract class SingletonMonoBehavior<T> : MonoBehaviour where T : MonoBehaviour
	{
		private static T instance;
		private static bool hasInstance;
		private static object lockObject = new object();


		public static T Instance
		{
			get
			{
				return GetInstance();
			}
		}


		static void CreateInstance()
		{
			var gameObject = new GameObject();
			gameObject.name = typeof(T).ToString();
			Debug.Log( "Creating instance of singleton: " + gameObject.name );
			instance = gameObject.AddComponent<T>();
			hasInstance = true;
		}


		static T GetInstance()
		{
			lock (lockObject)
			{
				if (hasInstance)
				{
					return instance;
				}

				var type = typeof(T);
				var objects = FindObjectsOfType<T>();

				if (objects.Length > 0)
				{
					instance = objects[0];
					hasInstance = true;

					if (objects.Length > 1)
					{
						Debug.LogWarning( "Multiple instances of singleton " + type + " found; destroying all but the first." );
						for (int i = 1; i < objects.Length; i++)
						{
							DestroyImmediate( objects[i].gameObject );
						}
					}

					return instance;
				}

				#if NETFX_CORE
				var attribute = type.GetTypeInfo().GetCustomAttribute<SingletonPrefabAttribute>();
				#else
				var attribute = Attribute.GetCustomAttribute( type, typeof(SingletonPrefabAttribute) ) as SingletonPrefabAttribute;
				#endif

				if (attribute == null)
				{
					CreateInstance();
				}
				else
				{
					var prefabName = attribute.Name;
					var gameObject = Instantiate( Resources.Load<GameObject>( prefabName ) ) as GameObject;
					if (gameObject == null)
					{
						Debug.LogError( "Could not find prefab " + prefabName + " for singleton of type " + type + "." );
						CreateInstance();
					}
					else
					{
						gameObject.name = prefabName;

						instance = gameObject.GetComponent<T>();
						if (instance == null)
						{
							Debug.LogWarning( "There wasn't a component of type \"" + type + "\" inside prefab \"" + prefabName + "\"; creating one now." );
							instance = gameObject.AddComponent<T>();
							hasInstance = true;
						}
					}
				}

				return instance;
			}
		}


		static void EnforceSingleton()
		{
			lock (lockObject)
			{
				if (hasInstance)
				{
					var objects = FindObjectsOfType<T>();
					for (int i = 0; i < objects.Length; i++)
					{
						if (objects[i].GetInstanceID() != instance.GetInstanceID())
						{
							DestroyImmediate( objects[i].gameObject );
						}
					}
				}
			}
		}


		protected bool SetupSingleton()
		{
			EnforceSingleton();
			return GetInstanceID() == Instance.GetInstanceID();
		}


		void OnDestroy()
		{
			hasInstance = false;
		}
	}
}