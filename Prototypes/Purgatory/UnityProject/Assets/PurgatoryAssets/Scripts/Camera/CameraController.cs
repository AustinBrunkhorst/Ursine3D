using UnityEngine;

public class CameraController : MonoBehaviour
{
    public GameObject target;
    public Vector3 positionOffset;
    public Vector3 rotation;

    void Update()
    {
        var targetPosition = target.transform.position + positionOffset;

        transform.position = targetPosition;
    }
}
