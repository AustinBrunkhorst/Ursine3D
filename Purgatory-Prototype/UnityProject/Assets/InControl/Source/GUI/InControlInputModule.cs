#if UNITY_4_6 || UNITY_5 || UNITY_5_0 || UNITY_5_1 || UNITY_5_2
using UnityEngine;
using UnityEngine.EventSystems;
using InControl;


namespace InControl
{
	[AddComponentMenu( "Event/InControl Input Module" )]
	public class InControlInputModule : PointerInputModule
	{
		public enum Button : int
		{
			Action1 = InputControlType.Action1,
			Action2 = InputControlType.Action2,
			Action3 = InputControlType.Action3,
			Action4 = InputControlType.Action4
		}

		public Button submitButton = Button.Action1;
		public Button cancelButton = Button.Action2;
		[Range( 0.1f, 0.9f )]
		public float analogMoveThreshold = 0.5f;
		public float moveRepeatFirstDuration = 0.8f;
		public float moveRepeatDelayDuration = 0.1f;
		public bool allowMobileDevice = true;
		public bool allowMouseInput = true;
		public bool focusOnMouseHover = false;

		InputDevice inputDevice;
		Vector3 thisMousePosition;
		Vector3 lastMousePosition;
		Vector2 thisVectorState;
		Vector2 lastVectorState;
		bool thisSubmitState;
		bool lastSubmitState;
		bool thisCancelState;
		bool lastCancelState;
		float nextMoveRepeatTime;
		float lastVectorPressedTime;
		TwoAxisInputControl direction;

		public PlayerAction SubmitAction { get; set; }
		public PlayerAction CancelAction { get; set; }
		public PlayerTwoAxisAction MoveAction { get; set; }


		protected InControlInputModule()
		{
			direction = new TwoAxisInputControl();
			direction.StateThreshold = analogMoveThreshold;
		}


		public override void UpdateModule()
		{
			lastMousePosition = thisMousePosition;
			thisMousePosition = Input.mousePosition;
		}


		public override bool IsModuleSupported()
		{
			#if UNITY_WII || UNITY_PS3 || UNITY_PS4 || UNITY_XBOX360 || UNITY_XBOXONE
			return true;
			#endif
			return allowMobileDevice || Input.mousePresent;
		}


		public override bool ShouldActivateModule()
		{
			if (!base.ShouldActivateModule())
			{
				return false;
			}

			UpdateInputState();

			var shouldActivate = false;
			shouldActivate |= SubmitWasPressed;
			shouldActivate |= CancelWasPressed;
			shouldActivate |= VectorWasPressed;

			if (allowMouseInput)
			{
				shouldActivate |= MouseHasMoved;
				shouldActivate |= MouseButtonIsPressed;
			}

			return shouldActivate;
		}


		public override void ActivateModule()
		{
			base.ActivateModule();
			thisMousePosition = Input.mousePosition;
			lastMousePosition = Input.mousePosition;

			var selectObject = eventSystem.currentSelectedGameObject;

			if (selectObject == null)
			{
				selectObject = eventSystem.firstSelectedGameObject;
			}

			eventSystem.SetSelectedGameObject( selectObject, GetBaseEventData() );
		}


		public override void DeactivateModule()
		{
			base.DeactivateModule();
			ClearSelection();
		}


		public override void Process()
		{
			var usedEvent = SendUpdateEventToSelectedObject();

			if (eventSystem.sendNavigationEvents)
			{
				if (!usedEvent)
				{
					usedEvent = SendVectorEventToSelectedObject();
				}

				if (!usedEvent)
				{
					SendButtonEventToSelectedObject();
				}
			}

			if (allowMouseInput)
			{
				ProcessMouseEvent();
			}
		}


		bool SendButtonEventToSelectedObject()
		{
			if (eventSystem.currentSelectedGameObject == null)
			{
				return false;
			}

			var eventData = GetBaseEventData();

			if (SubmitWasPressed)
			{
//				ExecuteEvents.Execute( eventSystem.currentSelectedGameObject, new PointerEventData( EventSystem.current ), ExecuteEvents.pointerDownHandler );
				ExecuteEvents.Execute( eventSystem.currentSelectedGameObject, eventData, ExecuteEvents.submitHandler );
			}
			else
			if (SubmitWasReleased)
			{
//				ExecuteEvents.Execute( eventSystem.currentSelectedGameObject, new PointerEventData( EventSystem.current ), ExecuteEvents.pointerUpHandler );
			}

			if (CancelWasPressed)
			{
				ExecuteEvents.Execute( eventSystem.currentSelectedGameObject, eventData, ExecuteEvents.cancelHandler );
			}

			return eventData.used;
		}


		bool SendVectorEventToSelectedObject()
		{
			if (!VectorWasPressed)
			{
				return false;
			}

			var axisEventData = GetAxisEventData( thisVectorState.x, thisVectorState.y, 0.5f );

			if (axisEventData.moveDir != MoveDirection.None)
			{
				if (eventSystem.currentSelectedGameObject == null)
				{
					eventSystem.SetSelectedGameObject( eventSystem.firstSelectedGameObject, GetBaseEventData() );
				}
				else
				{
					ExecuteEvents.Execute( eventSystem.currentSelectedGameObject, axisEventData, ExecuteEvents.moveHandler );
				}
				SetVectorRepeatTimer();
			}

			return axisEventData.used;
		}


		void ProcessMouseEvent()
		{
			var mouseData = GetMousePointerEventData();

			var pressed = mouseData.AnyPressesThisFrame();
			var released = mouseData.AnyReleasesThisFrame();

			var leftButtonData = mouseData.GetButtonState( PointerEventData.InputButton.Left ).eventData;

			if (!UseMouse( pressed, released, leftButtonData.buttonData ))
			{
				return;
			}

			// Process the first mouse button fully
			ProcessMousePress( leftButtonData );
			ProcessMove( leftButtonData.buttonData );
			ProcessDrag( leftButtonData.buttonData );

			// Now process right / middle clicks
			ProcessMousePress( mouseData.GetButtonState( PointerEventData.InputButton.Right ).eventData );
			ProcessDrag( mouseData.GetButtonState( PointerEventData.InputButton.Right ).eventData.buttonData );
			ProcessMousePress( mouseData.GetButtonState( PointerEventData.InputButton.Middle ).eventData );
			ProcessDrag( mouseData.GetButtonState( PointerEventData.InputButton.Middle ).eventData.buttonData );

			if (Utility.IsNotZero( leftButtonData.buttonData.scrollDelta.sqrMagnitude ))
			{
				var scrollHandler = ExecuteEvents.GetEventHandler<IScrollHandler>( leftButtonData.buttonData.pointerCurrentRaycast.gameObject );
				ExecuteEvents.ExecuteHierarchy( scrollHandler, leftButtonData.buttonData, ExecuteEvents.scrollHandler );
			}
		}


		private static bool UseMouse( bool pressed, bool released, PointerEventData pointerData )
		{
			if (pressed || released || pointerData.IsPointerMoving() || pointerData.IsScrolling())
			{
				return true;
			}
			return false;
		}


		bool SendUpdateEventToSelectedObject()
		{
			if (eventSystem.currentSelectedGameObject == null)
			{
				return false;
			}
			var eventData = GetBaseEventData();
			ExecuteEvents.Execute( eventSystem.currentSelectedGameObject, eventData, ExecuteEvents.updateSelectedHandler );
			return eventData.used;
		}


		private void ProcessMousePress( MouseButtonEventData data )
		{
			var pointerEvent = data.buttonData;
			var currentOverGo = pointerEvent.pointerCurrentRaycast.gameObject;

			// PointerDown notification
			if (data.PressedThisFrame())
			{
				pointerEvent.eligibleForClick = true;
				pointerEvent.delta = Vector2.zero;
				pointerEvent.dragging = false;
				pointerEvent.useDragThreshold = true;
				pointerEvent.pressPosition = pointerEvent.position;
				pointerEvent.pointerPressRaycast = pointerEvent.pointerCurrentRaycast;

				DeselectIfSelectionChanged( currentOverGo, pointerEvent );

				// search for the control that will receive the press
				// if we can't find a press handler set the press
				// handler to be what would receive a click.
				var newPressed = ExecuteEvents.ExecuteHierarchy( currentOverGo, pointerEvent, ExecuteEvents.pointerDownHandler );

				// didnt find a press handler... search for a click handler
				if (newPressed == null)
				{
					newPressed = ExecuteEvents.GetEventHandler<IPointerClickHandler>( currentOverGo );
				}

				float time = Time.unscaledTime;

				if (newPressed == pointerEvent.lastPress)
				{
					var diffTime = time - pointerEvent.clickTime;
					if (diffTime < 0.3f)
					{
						++pointerEvent.clickCount;
					}
					else
					{
						pointerEvent.clickCount = 1;
					}

					pointerEvent.clickTime = time;
				}
				else
				{
					pointerEvent.clickCount = 1;
				}

				pointerEvent.pointerPress = newPressed;
				pointerEvent.rawPointerPress = currentOverGo;

				pointerEvent.clickTime = time;

				// Save the drag handler as well
				pointerEvent.pointerDrag = ExecuteEvents.GetEventHandler<IDragHandler>( currentOverGo );

				if (pointerEvent.pointerDrag != null)
					ExecuteEvents.Execute( pointerEvent.pointerDrag, pointerEvent, ExecuteEvents.initializePotentialDrag );
			}

			// PointerUp notification
			if (data.ReleasedThisFrame())
			{
				ExecuteEvents.Execute( pointerEvent.pointerPress, pointerEvent, ExecuteEvents.pointerUpHandler );

				// See if we mouse up on the same element that we clicked on...
				var pointerUpHandler = ExecuteEvents.GetEventHandler<IPointerClickHandler>( currentOverGo );

				// PointerClick and Drop events
				if (pointerEvent.pointerPress == pointerUpHandler && pointerEvent.eligibleForClick)
				{
					ExecuteEvents.Execute( pointerEvent.pointerPress, pointerEvent, ExecuteEvents.pointerClickHandler );
				}
				else
				if (pointerEvent.pointerDrag != null)
				{
					ExecuteEvents.ExecuteHierarchy( currentOverGo, pointerEvent, ExecuteEvents.dropHandler );
				}

				pointerEvent.eligibleForClick = false;
				pointerEvent.pointerPress = null;
				pointerEvent.rawPointerPress = null;

				if (pointerEvent.pointerDrag != null && pointerEvent.dragging)
				{
					ExecuteEvents.Execute( pointerEvent.pointerDrag, pointerEvent, ExecuteEvents.endDragHandler );
				}

				pointerEvent.dragging = false;
				pointerEvent.pointerDrag = null;

				// Redo pointer enter / exit to refresh state
				// so that if we moused over somethign that ignored it before
				// due to having pressed on something else
				// it now gets it.
				if (currentOverGo != pointerEvent.pointerEnter)
				{
					HandlePointerExitAndEnter( pointerEvent, null );
					HandlePointerExitAndEnter( pointerEvent, currentOverGo );
				}
			}
		}


		protected override void ProcessMove( PointerEventData pointerEvent )
		{
			var lastPointerEnter = pointerEvent.pointerEnter;

			base.ProcessMove( pointerEvent );

			if (focusOnMouseHover && lastPointerEnter != pointerEvent.pointerEnter)
			{
				var selectObject = ExecuteEvents.GetEventHandler<ISelectHandler>( pointerEvent.pointerEnter );
				eventSystem.SetSelectedGameObject( selectObject, pointerEvent );
			}
		}


		void Update()
		{
			direction.Filter( Device.Direction, Time.deltaTime );
		}


		void UpdateInputState()
		{
			lastVectorState = thisVectorState;
			thisVectorState = Vector2.zero;

			TwoAxisInputControl dir = MoveAction ?? direction;

			if (Utility.AbsoluteIsOverThreshold( dir.X, analogMoveThreshold ))
			{
				thisVectorState.x = Mathf.Sign( dir.X );
			}

			if (Utility.AbsoluteIsOverThreshold( dir.Y, analogMoveThreshold ))
			{
				thisVectorState.y = Mathf.Sign( dir.Y );
			}

			if (VectorIsReleased)
			{
				nextMoveRepeatTime = 0.0f;
			}

			if (VectorIsPressed)
			{
				if (lastVectorState == Vector2.zero)
				{
					if (Time.realtimeSinceStartup > lastVectorPressedTime + 0.1f)
					{
						nextMoveRepeatTime = Time.realtimeSinceStartup + moveRepeatFirstDuration;
					}
					else
					{
						nextMoveRepeatTime = Time.realtimeSinceStartup + moveRepeatDelayDuration;
					}
				}

				lastVectorPressedTime = Time.realtimeSinceStartup;
			}

			lastSubmitState = thisSubmitState;
			thisSubmitState = SubmitAction == null ? SubmitButton.IsPressed : SubmitAction.IsPressed;

			lastCancelState = thisCancelState;
			thisCancelState = CancelAction == null ? CancelButton.IsPressed : CancelAction.IsPressed;
		}


		InputDevice Device
		{
			set
			{
				inputDevice = value;
			}

			get
			{
				return inputDevice ?? InputManager.ActiveDevice;
			}
		}


		InputControl SubmitButton
		{
			get
			{
				return Device.GetControl( (InputControlType) submitButton );
			}
		}


		InputControl CancelButton
		{
			get
			{
				return Device.GetControl( (InputControlType) cancelButton );
			}
		}


		void SetVectorRepeatTimer()
		{
			nextMoveRepeatTime = Mathf.Max( nextMoveRepeatTime, Time.realtimeSinceStartup + moveRepeatDelayDuration );
		}


		bool VectorIsPressed
		{
			get
			{
				return thisVectorState != Vector2.zero;
			}
		}


		bool VectorIsReleased
		{
			get
			{
				return thisVectorState == Vector2.zero;
			}
		}


		bool VectorHasChanged
		{
			get
			{
				return thisVectorState != lastVectorState;
			}
		}


		bool VectorWasPressed
		{
			get
			{
				if (VectorIsPressed && Time.realtimeSinceStartup > nextMoveRepeatTime)
				{
					return true;
				}

				return VectorIsPressed && lastVectorState == Vector2.zero;
			}
		}


		bool SubmitWasPressed
		{
			get
			{
				return thisSubmitState && thisSubmitState != lastSubmitState;
			}
		}


		bool SubmitWasReleased
		{
			get
			{
				return !thisSubmitState && thisSubmitState != lastSubmitState;
			}
		}


		bool CancelWasPressed
		{
			get
			{
				return thisCancelState && thisCancelState != lastCancelState;
			}
		}


		bool MouseHasMoved
		{
			get
			{
				return (thisMousePosition - lastMousePosition).sqrMagnitude > 0.0f;
			}
		}


		bool MouseButtonIsPressed
		{
			get
			{
				return Input.GetMouseButtonDown( 0 );
			}
		}
	}
}
#endif

