using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bee : MonoBehaviour
{
    public Vector3 mousePos3D;
    public Vector3 lastPos; // Used as a buffer location for transform rotation.
    public Vector3 lastLastPos; // Used as a reference point for bee rotation as it follows the mouse.
    public Vector3 relativeMousePos;
    public GameObject SpawnPoint;
    public float speed;
    public float angle;
    public bool asleep = true; // If true, Bee will follow the player's mouse input. If false, Bee will remain stationary.
    // Start is called before the first frame update
    void Start()
    {
        SpawnPoint = GameObject.FindGameObjectWithTag("BeeSpawnPoint");
        transform.position = SpawnPoint.transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        // Convert the position of the mouse pointer to a point in the game.
        Vector3 mousePos2D = Input.mousePosition;
        mousePos2D.z = -Camera.main.transform.position.z;
        mousePos3D = Camera.main.ScreenToWorldPoint(mousePos2D);

        // Once the mouse is close enough to the Bee, it will start following the mouse as it moves.
        // This ensures that the Bee won't be placed out of bounds when it spawns.
        if( Vector3.Distance(mousePos3D, transform.position) < 1.0f )
        {
            asleep = false;
            GameManager.S.Canvas_ToolTip.gameObject.SetActive(false);
        }

        // This if statement blocks the Bee's movement until the player has put the mouse close enough to it.
        if ( !asleep && GameManager.S.isTimerActive) { 
            // Blocks the Bee's movement if there is an obstacle between it and the mouse.
            if ( !Physics.Linecast(transform.position, mousePos3D) ) {

                transform.position = Vector3.MoveTowards( transform.position, mousePos3D, speed); // Using MoveTowards because it allows for smoother movement of the Bee.

                relativeMousePos.x = transform.position.x - lastLastPos.x;
                relativeMousePos.y = transform.position.y - lastLastPos.y;

                angle = Mathf.Atan2(relativeMousePos.y, relativeMousePos.x) * Mathf.Rad2Deg; // Calculates the angle between the Bee and where it's moved from.
                transform.rotation = Quaternion.Euler(new Vector3(0, 0, angle + 270)); // Rotates the Bee so that it points in the direction it's moving.
            }
        }
        
    }

    private void FixedUpdate()
    {
        // Updates the Bee's previous locations every time it moves by .25
        if( Vector3.Distance(lastPos, transform.position) > .25f)
        {
            lastLastPos = lastPos;
            lastPos = transform.position;
        }
        
    }

    private void OnTriggerEnter(Collider other)
    {
        // If the player touches a Red Zone or Bomb, reset it's position to spawn and lose a life.
        if (other.gameObject.tag == "RedZone" || other.gameObject.tag == "Bomb")
        {
            transform.position = SpawnPoint.transform.position;
            asleep = true;
            GameManager.S.LoseLife();
        }
    }
}
