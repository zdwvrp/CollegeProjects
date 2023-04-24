using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Balloon : MonoBehaviour
{

    public GameObject CheckmarkPrefab;
    // Start is called before the first frame update
    void Awake()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Bee")
        {
            Pop();
        }
    }

    void Pop()
    {
        Instantiate(CheckmarkPrefab, transform.position, transform.rotation);
        GameManager.S.BalloonPopped(gameObject.name);
    }
}
