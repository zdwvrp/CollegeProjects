using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BezierExampleCubic : MonoBehaviour
{
    public GameObject P0;
    public GameObject P1;
    public GameObject C0;
    public GameObject C1;
    public GameObject B;

    public Vector3 p0pos;
    public Vector3 p1pos;
    public Vector3 c0pos;
    public Vector3 c1pos;
    public Vector3 bpos;

    public LineRenderer line;

    float timeStart;
    float u;
    public float timeDuration = 1f;
    // Start is called before the first frame update
    void Start()
    {
        timeStart = Time.time;
        p0pos = P0.transform.position;
        p1pos = P1.transform.position;
        c0pos = C0.transform.position;
        c1pos = C1.transform.position;
        bpos = B.transform.position;

        line = GetComponent<LineRenderer>();
        u = 0;
    }

    // Update is called once per frame
    void Update()
    {
        
        if( u < 1f)
        {
            u += 0.001f;
            Vector3 d = ( (1 - u) * (1 - u) * (1 - u) * p0pos ) + ( 3 * (1 - u) * (1 - u) * u * c0pos ) + (3 * (1 - u) * u * u * c1pos ) + (u * u * u * p1pos);

            transform.position = d;

            line.positionCount++;
            line.SetPosition(line.positionCount - 1, d);
        }
    }
}
