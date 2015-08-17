using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using UnityEngine;


namespace InControl
{
	public class PlayerOneAxisAction : OneAxisInputControl
	{
		PlayerAction negativeAction;
		PlayerAction positiveAction;


		internal PlayerOneAxisAction( PlayerAction negativeAction, PlayerAction positiveAction )
		{
			this.negativeAction = negativeAction;
			this.positiveAction = positiveAction;

			Raw = true;
		}


		internal void Update( ulong updateTick, float deltaTime )
		{
			var value = Utility.ValueFromSides( negativeAction, positiveAction );
			CommitWithValue( value, updateTick, deltaTime );
		}
	}
}

