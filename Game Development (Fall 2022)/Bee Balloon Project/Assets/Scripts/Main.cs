using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Main : MonoBehaviour
{
    public static Main S;

    public Canvas Canvas_LevelComplete; // Canvas GameObject that displays when the level is complete.

    public int currentLevelIndex = 0;
    public string[] levelSceneNamesInOrder = { "_Scene_0", "_Scene_1", "_Scene_2", "_Scene_3" };

    public int lives = 3; // Number of lives the player has.
    public int points = 0; // Number of points the player has.
    public float timer = 240; // Timer.
    public float initialTime = 240;

    private void Awake()
    {
        S = this;
        initialTime = timer;
        DontDestroyOnLoad(S);
        DontDestroy();

        PlayerPrefs.SetInt("score", 0);
    }

    
    public void ChangeScene()
    {
        if( currentLevelIndex < levelSceneNamesInOrder.Length )
        {
            SceneManager.LoadScene(levelSceneNamesInOrder[currentLevelIndex]);
            currentLevelIndex++;

        } else
        {
            SceneManager.LoadScene("_End_Screen");
        }
        
        
    }

    public void RestartGame()
    {
        SceneManager.LoadScene("_Start_Screen");
        ResetScore();
    }

    public void SetScore()
    {
        PlayerPrefs.SetInt("score", points);
    }

    public void ResetScore()
    {
        PlayerPrefs.SetInt("score", 0);
    }

    private void DontDestroy()
    {
        DontDestroyOnLoad(Canvas_LevelComplete);
    }
}
