using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public enum GameStatus
{
    active,
    changingLevel,
    levelLoading,
    inactive
}


public class GameManager : MonoBehaviour
{
    public static GameManager S;

    [Header("Set in Inspector")]
    public GameObject BeePrefab;
    public Image Image_ShowDamage;
    public Text Text_Lives; // Text GameObject that displays lives.
    public Text Text_Timer; // Text GameObject that displays time.
    public Image Image_Timer;
    public Text Text_Points; // Text GameObject that displays points.
    public Text Text_Level;
    public Text Text_BalloonsRemaining;
    public Canvas Canvas_ToolTip;
    

    [Header("Set Dynamically")]
    public List<GameObject> balloons; // List that contains all balloons present in the level.
    public bool isTimerActive = false;
    public GameStatus status;


    private float showDamageColorAlpha = .25f;

    private void Awake()
    { 
        S = this; // Initialize singleton.
        status = GameStatus.inactive;
        PlayerPrefs.SetInt("score", 0);
        StartLevel();

        Text_Lives.text = "Lives: " + Main.S.lives;
        Text_Points.text = "Points: " + Main.S.points;
        Text_Level.text = "Level " + Main.S.currentLevelIndex;
    }

    // Update is called once per frame
    void Update()
    {
        if (isTimerActive)
        {
            Main.S.timer -= Time.deltaTime;
            float minutes = Mathf.Floor(Main.S.timer / 60);
            float seconds = Mathf.Floor(Main.S.timer % 60);
            Text_Timer.text = minutes + ":" + seconds;
            Image_Timer.fillAmount = Main.S.timer / Main.S.initialTime;

            if (seconds < 10)
            {
                Text_Timer.text = minutes + ":" + "0" + seconds;
            }

            if (Image_ShowDamage.color.a > 0)
            {
                Color c = Image_ShowDamage.color;
                c.a -= Time.deltaTime / 2f;
                Image_ShowDamage.color = c;
            } else
            {
                Color c = Image_ShowDamage.color;
                c.a = 0;
                Image_ShowDamage.color = c;
            }
            

            // Once all balloons are destroyed, the level is won.
            if (balloons.Count == 0 && status == GameStatus.active)
            {
                LevelComplete();
            }

            // Once the player is out of lives, the game is lost.
            if ( (Main.S.lives <= 0 || Main.S.timer <= 0) && status == GameStatus.active )
            {
                SceneManager.LoadScene("_End_Screen");
            }
        }
    }

    public void StartLevel()
    {
        InitializeBalloons();
        Instantiate(BeePrefab);
        status = GameStatus.active;
        isTimerActive = true;
        Time.timeScale = 1;
    }

    public void BalloonPopped(string name)
    {
        // Searches through the balloons list to find a specific balloon.
        // That balloon is then removed from the list and destroyed.f
        foreach(GameObject balloon in balloons)
        {
            if (balloon.name == name)
            {
                balloons.Remove(balloon);
                Destroy(balloon);
                AddPoints(1);
                Text_BalloonsRemaining.text = "Balloons Left: " + balloons.Count;
                break;
            }
        }
    }

    // Function used to decrement the lives variable.
    // Can be called in other classes.
    public void LoseLife()
    {
        Main.S.lives--;
        Text_Lives.text = "Lives: " + Main.S.lives;
        Color c = Image_ShowDamage.color;
        c.a = showDamageColorAlpha;
        Image_ShowDamage.color = c;
    }

    public void AddPoints(int pointsToAdd)
    {
        Main.S.points += pointsToAdd;
        Text_Points.text = "Points: " + Main.S.points;
        Main.S.SetScore();
    }


    private void LevelComplete()
    {
        Time.timeScale = 0;
        status = GameStatus.inactive;
        isTimerActive = false;
        Main.S.Canvas_LevelComplete.gameObject.SetActive(true);
    }

    private void InitializeBalloons()
    {
        balloons.Clear();
        GameObject[] balloonArray = GameObject.FindGameObjectsWithTag("Balloon");
        foreach (GameObject balloon in balloonArray)
        {
            Color c = new Color(Random.value, Random.value, Random.value);
            Renderer balloonRenderer = balloon.GetComponent<Renderer>();
            balloonRenderer.material.color = c;
            balloons.Add(balloon);
        }

        Text_BalloonsRemaining.text = "Balloons Left: " + balloons.Count;
    }
}


// Config:
// Add "Balloon", "RedZone", "Bee", and "Wall" layers in project settings
// Add all scenes to File>Build Settings>Setting in Build

//**********************************************************

// TODO:

// - Create 4 levels
//      - Make a new scene for each level

// - Create a pause button

// - add "brief introduction how the game is played, developer names, a start button, as well as the
//   levels the player last played" to start scene
//      - last levels could be dont with a playerPref

// "Environment has a distinct theme, with each element is purposeful and meaningful for the scene"






