using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{

    [Header("Set in Inspector")]
    public GameObject BeePrefab;


    // Start is called before the first frame update
    void Start()
    {
        StartLevel();
    }

    // Update is called once per frame
    void Update()
    {
    }

    private void StartLevel()
    {
        Instantiate(BeePrefab);
    }
}
