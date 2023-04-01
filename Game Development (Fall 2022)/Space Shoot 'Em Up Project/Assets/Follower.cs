using PathCreation;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Follower : MonoBehaviour
{
    public PathCreator pathCreator;
    public float speed = 5;
    float distanceTrav;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        distanceTrav += speed * Time.deltaTime;
        transform.position = pathCreator.path.GetPointAtDistance(distanceTrav);
        transform.rotation = pathCreator.path.GetRotationAtDistance(distanceTrav);
    }
}
