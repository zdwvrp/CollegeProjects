using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bee : MonoBehaviour
{
    public Vector3 mousePos3D;
    public Vector3 lastValidPosition;
    public bool isTouchingAWall = false;
    // Start is called before the first frame update
    void Start()
    {
        lastValidPosition = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 mousePos2D = Input.mousePosition;
        mousePos2D.z = -Camera.main.transform.position.z;
        mousePos3D = Camera.main.ScreenToWorldPoint(mousePos2D);

        if ( !Physics.Linecast(transform.position, mousePos3D) ) {
            isTouchingAWall = true;
            transform.position = mousePos3D;
        } else
        {
            isTouchingAWall = false;
        }
    }

/*    private void OnCollisionExit(Collision collision)
    {

        isTouchingAWall = false;
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Wall")
        {
            Debug.Log("Hit a wall");
            isTouchingAWall = true;
            //lastValidPosition = transform.position;
            //isInBounds = false;
        }
    }*/
}
