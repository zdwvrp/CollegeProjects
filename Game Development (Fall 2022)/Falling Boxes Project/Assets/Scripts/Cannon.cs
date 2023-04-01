using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class Cannon : MonoBehaviour
{
    [Header("Set in Inspector")]
    public GameObject cannonBallPrefab1;
    public GameObject cannonBallPrefab2;
    public GameObject cannonBallPrefab3;
    public GameObject cannonBarrel;
    public float projectileSpeed = 40;

    [Header("Set Dynamically")]
    public Vector3 mousePos3D;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Main.S.gameMode == GameMode.playing)
        {
            Vector3 mousePos2D = Input.mousePosition;
            mousePos2D.z = -Camera.main.transform.position.z;
            mousePos3D = Camera.main.ScreenToWorldPoint(mousePos2D);
            Vector3 direction = mousePos3D - transform.position;

            float angle = Mathf.Rad2Deg * Mathf.Atan2(direction.y, direction.x);

            transform.rotation = Quaternion.Euler(0, 0, -(90 - angle));

            if (Input.GetKeyDown(KeyCode.S))
            {
                if (Main.S.shotOneCount > 0)
                {
                    CannonFire(mousePos3D, cannonBallPrefab1);
                    Main.S.TakeShotOne();
                } 
            }

            if (Input.GetKeyDown(KeyCode.D))
            {
                if (Main.S.shotTwoCount > 0)
                {
                    CannonFire(mousePos3D, cannonBallPrefab2);
                    Main.S.TakeShotTwo();
                } 

            }

            if (Input.GetKeyDown(KeyCode.F))
            {
                if (Main.S.shotThreeCount > 0)
                {
                    CannonFire(mousePos3D, cannonBallPrefab3);
                    Main.S.TakeShotThree();
                } 
            }
        }
    }

    void CannonFire(Vector3 mousePos, GameObject cannonBall)
    {
        GameObject projGO = Instantiate<GameObject>(cannonBall);
        projGO.transform.position = cannonBarrel.transform.position;
        Rigidbody rigidB = projGO.GetComponent<Rigidbody>();
        mousePos.Normalize();
        rigidB.velocity = mousePos * projectileSpeed;
        
    }
}
