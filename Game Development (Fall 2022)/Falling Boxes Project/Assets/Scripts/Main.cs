using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.UIElements;
using Image = UnityEngine.UI.Image;

public enum GameMode
{
    idle,
    playing,
    levelUp,
    gameOver
}

public class Main : MonoBehaviour
{
    static public Main S;
    [Header("Set in Inspector - Text Fields")]
    public Text timerText;
    public Text levelText;
    public Text scoreText;
    public Text nextLevelScoreText;
    public Text shotOneText;
    public Text shotTwoText;
    public Text shotThreeText;
    public Text levelUpText;
    public Canvas levelUpCanvas;
    public Image levelUpBackground;

    [Header("Set in Inspector")]
    public GameObject[] prefabBoxes;
    public GameObject boxDisplayScore;
    public float boxSpawnPerSecond = 0.5f;
    public float boxDefaultPadding = 1.5f;
    public float horizontalBounds = 9.75f;
    public float spawnHeight = 12.5f;
    public int defaultShotCount = 50;
    public float timePerLevel = 120f;
    public float levelUpTimer = 5f;

    [Header("Set Dynamically")]
    public int score = 0;
    public int level = 0;
    public int scoreToNextLevel;
    public int shotOneCount;
    public int shotTwoCount;
    public int shotThreeCount;
    public float boxSpeedMultiplier = 1f;
    public float timer;
    public GameMode gameMode = GameMode.idle;

    // Score values required to increase level
    private int[] levelScoreValues = new int[] { 100, 200, 350, 400, 600 };
    public float[] levelBoxSpeeds = new float[] { 1f, 1.5f, 2f, 2.25f, 2.75f };
    public float[] levelBoxSpawnRates = new float[] { .5f, .45f, .4f, .375f, .35f };
    private int maxLevel;


    private void Awake()
    {
        S = this;
        maxLevel = S.levelScoreValues.Length - 1;
        StartLevel();
    }

    // Update is called once per frame
    void Update()
    {
        // Update Timer
        if (gameMode == GameMode.playing)
        {
            timer -= Time.deltaTime;
        }

        if (timer <= 0 || shotOneCount + shotTwoCount + shotThreeCount == 0)
        {
            gameMode = GameMode.gameOver;
            EndGame();
        }

        // Update UI Text
        timerText.text = Mathf.Round(timer).ToString();
        levelText.text = (level + 1).ToString();
        scoreText.text = score.ToString();
        shotOneText.text = shotOneCount.ToString();
        shotTwoText.text = shotTwoCount.ToString();
        shotThreeText.text = shotThreeCount.ToString();

        if (level < maxLevel)
        {
            nextLevelScoreText.text = scoreToNextLevel + " => Lvl " + (level + 1);
        }
        else
        {
            nextLevelScoreText.text = "Max Level!";
        }


    }
    private void FixedUpdate()
    {
        if (score >= levelScoreValues[level] && level < maxLevel)
        {
            LevelUp();
        }
    }

    public void SpawnBox()
    {
        int boxType = Random.Range(0, 4);
        GameObject box = Instantiate<GameObject>(prefabBoxes[boxType]);

        float posX = Random.Range(-horizontalBounds, horizontalBounds);
        Vector3 pos = new Vector3(posX, spawnHeight, 0);

        box.transform.position = pos;

        // Continue to spawn boxes iff player hasn't leveled up
        if (gameMode == GameMode.playing)
        {
            Invoke("SpawnBox", 2f * boxSpawnPerSecond);
        }
    }

    public void DisplayScore(Vector3 pos, int score, bool addOrSubtract)
    {
        GameObject scoreDisplay = Instantiate<GameObject>(boxDisplayScore);
        Text scoreDisplayText = scoreDisplay.GetComponentInChildren<Text>();
        scoreDisplay.transform.position = pos;

        if (addOrSubtract)
        {
            scoreDisplayText.text = "+" + score;
        }
        else
        {
            scoreDisplayText.text = "-" + score;
        }
    }

    private void StartLevel()
    {
        // Set scoreToNextLevel
        scoreToNextLevel = S.levelScoreValues[level];

        // Set shots, box speed and timer to default values
        shotOneCount = shotTwoCount = shotThreeCount = defaultShotCount;
        boxSpeedMultiplier = S.levelBoxSpeeds[level];
        boxSpawnPerSecond = S.levelBoxSpawnRates[level];
        timer = timePerLevel;

        // Start the level
        gameMode = GameMode.playing;
        Invoke("SpawnBox", 1f / boxSpawnPerSecond);

        // Remove Level Up UI
        levelUpCanvas.enabled = false;
    }

    private void LevelUp()
    {
        gameMode = GameMode.levelUp;
        level++;

        levelUpCanvas.enabled = true;
        levelUpText.text = "Ammo Refilled\nTimer Reset\nReach " + S.levelScoreValues[level] + " Score for the next Level!";

        // Start the Next Level
        Invoke("StartLevel", levelUpTimer);
    }

    private void EndGame()
    {
        StateManager.S.SetFinalScore(score);
        SceneHandler.EndGame();
    }

    public void AddScore(int points)
    {
        score += points;
    }

    public void SubtractScore(int points)
    {
        score -= points;
    }

    public void TakeShotOne()
    {
        shotOneCount--;
    }

    public void TakeShotTwo()
    {
        shotTwoCount--;
    }

    public void TakeShotThree()
    {
        shotThreeCount--;
    }
}