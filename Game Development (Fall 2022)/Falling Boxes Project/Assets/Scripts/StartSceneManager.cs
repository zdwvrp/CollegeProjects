using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class StartSceneManager : MonoBehaviour
{
    public Text HighScoreText;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        HighScoreText.text = StateManager.S.highScore.ToString();
    }
}
