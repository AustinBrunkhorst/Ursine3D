using System;
using System.IO;
using UnityEngine;


namespace InControl
{
	public enum InputRangeType : int
	{
		None = 0,
		MinusOneToOne,
		ZeroToOne,
		ZeroToMinusOne,
		ZeroToNegativeInfinity,
		ZeroToPositiveInfinity,
		Everything
	}
}
