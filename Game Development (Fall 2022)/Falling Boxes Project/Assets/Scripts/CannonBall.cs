using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CannonBall : MonoBehaviour
{
    public float maxHeight = 12f;

    public Rigidbody rb;
    private void Start()
    {
        rb = gameObject.GetComponent<Rigidbody>();
    }
    void Update()
    {
        if (gameObject.transform.localPosition.y > maxHeight)
        { 
            Destroy(gameObject);
        }
    }
}
