using InControl;
using UnityEngine;

[RequireComponent(typeof(Rigidbody), typeof(Animator))]
public class PlayerMovement : MonoBehaviour
{
    [Range(1.0f, 4.0f)]
    public float baseMoveSpeed = 1.0f;
    [Range(1.0f, 5.0f)]
    public float runSpeedModifier = 2f;

    private PlayerControlScheme controls;

    private float speedMod;
    private Vector3 moveDir;

    // Component Dependencies
    private new Rigidbody rigidbody;
    private Animator animator;

    void Awake()
    {
        controls = new PlayerControlScheme();
        rigidbody = GetComponent<Rigidbody>();
        animator = GetComponent<Animator>();
    }

    void Update()
    {
        controls.Device = InputManager.ActiveDevice;

        // player movement input
        moveDir.x = controls.Move.Value.x;
        moveDir.z = controls.Move.Value.y;

        // rotate the player
        if (moveDir.sqrMagnitude >= Mathf.Epsilon * Mathf.Epsilon)
        {
            moveDir.Normalize();

            transform.rotation = Quaternion.Euler(
                new Vector3(
                    0.0f, 
                    90 + Mathf.Atan2(moveDir.x, moveDir.z) * Mathf.Rad2Deg,
                    0.0f
                )
            );

            animator.SetBool("Moving", true);
        }
        else
        {
            animator.SetBool("Moving", false);
        }

        // player running input
        if (controls.Run.IsPressed)
        {
            speedMod = runSpeedModifier;
            animator.SetBool("Running", true);
        }
        else
        {
            speedMod = 1.0f;
            animator.SetBool("Running", false);
        }
    }

    void FixedUpdate()
    {
        rigidbody.AddForce(
            moveDir * baseMoveSpeed * speedMod,
            ForceMode.Force
        );
    }
}
