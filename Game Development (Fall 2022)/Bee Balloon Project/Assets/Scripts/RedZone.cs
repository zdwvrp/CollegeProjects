using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RedZone : MonoBehaviour
{
    public bool moveVertical;
    public bool moveHorizontal;
    public bool shouldRotate;
    public float rotationalSpeed;

    public int spawnPointX = 0;
    public int spawnPointY = 0;

    public float YTopBound;
    public float YBottomBound;

    public float XLeftBound;
    public float XRightBound;

    public float speed = 10;

    //level 1 Ytop: 10.5F, YBot:8.5F

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (moveVertical == true)
        {
            // Basic Movement
            Vector3 pos = transform.position;
            pos.y += speed * Time.deltaTime;
            transform.position = pos;

            // Changing Direction
            if (pos.y < -YTopBound)
            {                             
                speed = Mathf.Abs(speed); // Move up                    
            }
            else if (pos.y > YBottomBound)
            {                       
                speed = -Mathf.Abs(speed); // Move down                    
            }
        }

        if (moveHorizontal == true)
        {
            // Basic Movement
            Vector3 pos = transform.position;
            pos.x += speed * Time.deltaTime;
            transform.position = pos;

            // Changing Direction
            if (pos.x < -XRightBound)
            {
                speed = Mathf.Abs(speed); // Move up                    
            }
            else if (pos.x > XLeftBound)
            {
                speed = -Mathf.Abs(speed); // Move down                    
            }
        }

        if (shouldRotate == true)
        {
            Vector3 rotationToAdd = new Vector3(0, 0, speed * Time.deltaTime);
            transform.Rotate(rotationToAdd);
        }
    }
}
