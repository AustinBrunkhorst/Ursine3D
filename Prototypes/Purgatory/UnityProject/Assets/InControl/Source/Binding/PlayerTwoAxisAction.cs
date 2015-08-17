using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using UnityEngine;


namespace InControl
{
	public class PlayerTwoAxisAction : TwoAxisInputControl
	{
		PlayerAction negativeXAction;
		PlayerAction positiveXAction;
		PlayerAction negativeYAction;
		PlayerAction positiveYAction;

		/// <summary>
		/// Gets or sets a value indicating whether the Y axis should be inverted for
		/// this action. When false (default), the Y axis will be positive up,
		/// the same as Unity.
		/// </summary>
		public bool InvertYAxis { get; set; }


		internal PlayerTwoAxisAction( PlayerAction negativeXAction, PlayerAction positiveXAction, PlayerAction negativeYAction, PlayerAction positiveYAction )
		{
			this.negativeXAction = negativeXAction;
			this.positiveXAction = positiveXAction;
			this.negativeYAction = negativeYAction;
			this.positiveYAction = positiveYAction;

			InvertYAxis = false;
			Raw = true;
		}


		internal void Update( ulong updateTick, float deltaTime )
		{
			var x = Utility.ValueFromSides( negativeXAction, positiveXAction, false );
			var y = Utility.ValueFromSides( negativeYAction, positiveYAction, InputManager.InvertYAxis || InvertYAxis );
			UpdateWithAxes( x, y, updateTick, deltaTime );
		}
	}
}
