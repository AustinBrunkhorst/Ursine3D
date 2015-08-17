using UnityEngine;
using InControl;


namespace InterfaceMovement
{
	public class ButtonManager : MonoBehaviour
	{
		public Button focusedButton;

		TwoAxisInputControl filteredDirection;


		void Awake()
		{
			filteredDirection = new TwoAxisInputControl();
			filteredDirection.StateThreshold = 0.5f;
		}


		void Update()
		{
			// Use last device which provided input.
			var inputDevice = InputManager.ActiveDevice;
			filteredDirection.Filter( inputDevice.Direction, Time.deltaTime );
//			filteredDirection = inputDevice.Direction;

			if (filteredDirection.Left.WasRepeated)
			{
				Debug.Log( "!!!" );
			}

			// Move focus with directional inputs.
			if (filteredDirection.Up.WasPressed)
			{
				MoveFocusTo( focusedButton.up );
			}
			
			if (filteredDirection.Down.WasPressed)
			{
				MoveFocusTo( focusedButton.down );
			}
			
			if (filteredDirection.Left.WasPressed)
			{
				MoveFocusTo( focusedButton.left );
			}
			
			if (filteredDirection.Right.WasPressed)
			{
				MoveFocusTo( focusedButton.right );
			}
		}
		
		
		void MoveFocusTo( Button newFocusedButton )
		{
			if (newFocusedButton != null)
			{
				focusedButton = newFocusedButton;
			}
		}
	}
}