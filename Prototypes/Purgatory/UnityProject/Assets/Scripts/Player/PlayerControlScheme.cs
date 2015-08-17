using UnityEngine;
using InControl;

public class PlayerControlScheme : PlayerActionSet
{
    private PlayerAction moveUp;
    private PlayerAction moveDown;
    private PlayerAction moveRight;
    private PlayerAction moveLeft;
    public PlayerTwoAxisAction Move;
    public PlayerAction Run;

    public PlayerControlScheme()
    {
        moveUp = CreatePlayerAction("moveUp");
        moveDown = CreatePlayerAction("moveDown");
        moveRight = CreatePlayerAction("moveRight");
        moveLeft = CreatePlayerAction("moveLeft");
        Move = CreateTwoAxisPlayerAction(moveLeft, moveRight, moveDown, moveUp);
        Move.LowerDeadZone = 0.25f;
        Run = CreatePlayerAction("Run");

        // Controller
        moveUp.AddDefaultBinding(InputControlType.LeftStickUp);
        moveDown.AddDefaultBinding(InputControlType.LeftStickDown);
        moveRight.AddDefaultBinding(InputControlType.LeftStickRight);
        moveLeft.AddDefaultBinding(InputControlType.LeftStickLeft);
        Run.AddDefaultBinding(InputControlType.Action1);

        // Keyboard
        moveUp.AddDefaultBinding(Key.W);
        moveDown.AddDefaultBinding(Key.S);
        moveRight.AddDefaultBinding(Key.D);
        moveLeft.AddDefaultBinding(Key.A);
        Run.AddDefaultBinding(Key.Space);
    }

    ~PlayerControlScheme()
    {
        Destroy();
    }
}
