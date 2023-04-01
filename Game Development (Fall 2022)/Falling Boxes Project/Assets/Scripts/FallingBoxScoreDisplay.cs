using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FallingBoxScoreDisplay : MonoBehaviour
{
    public FallingBoxScoreDisplay S;
    public float TimeVisible = 1f;
    public float time;
    public int score;
    // Start is called before the first frame update
    void Start()
    {
        S = this;
        time = TimeVisible;
    }

    // Update is called once per frame
    void Update()
    {
        time -= Time.deltaTime;
        if( time <= 0 )
        {
            Destroy(gameObject);
        }
    }

    public void setScore(int score)
    {
        this.score = score;
    }
}
