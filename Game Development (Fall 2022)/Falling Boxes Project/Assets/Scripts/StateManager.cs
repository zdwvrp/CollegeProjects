using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class StateManager : MonoBehaviour
{
    static public StateManager S;
    public int highScore;
    public int finalScore;
    public bool newHighScore = false;

    // Start is called before the first frame update
    void Start()
    {
        S = this;
        GetHighScore();
        DontDestroyOnLoad(S);
    }

    // Update is called once per frame
    void Update()
    {
        if( finalScore > highScore )
        {
            PlayerPrefs.SetInt("highScore", highScore);
        }
    }

    private void GetHighScore()
    {
        // Retrieve High Score if it exists
        if (PlayerPrefs.HasKey("highScore"))
        {
            highScore = PlayerPrefs.GetInt("highScore");
        }
        else
        {
            highScore = 0;
            PlayerPrefs.SetInt("highScore", highScore);
        }
    }

    public void SetFinalScore( int newScore )
    {
        finalScore = newScore;
        if( finalScore > highScore )
        {
            highScore = finalScore;
            PlayerPrefs.SetInt("highScore", highScore);
            newHighScore = true;
        }
    }

    public void ResetGame()
    {
        newHighScore = false;
        GetHighScore();
    }
}
