using System;
using UnityEngine;


namespace InControl
{
	public interface IInputControl
	{
		bool HasChanged { get; }
		bool IsPressed { get; }
		bool WasPressed { get; }
		bool WasReleased { get; }
	}
}

