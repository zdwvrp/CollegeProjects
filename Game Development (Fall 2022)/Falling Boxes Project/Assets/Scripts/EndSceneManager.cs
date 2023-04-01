using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class EndSceneManager : MonoBehaviour
{
    public Text HighScoreText;
    public Text FinalScoreText;
    public Text NewHighScoreText;
    // Start is called before the first frame update
    void Start()
    {
        NewHighScoreText.enabled = false;
    }

    // Update is called once per frame
    void Update()
    {
        HighScoreText.text = StateManager.S.highScore.ToString();
        FinalScoreText.text = StateManager.S.finalScore.ToString();
        if( StateManager.S.newHighScore )
        {
            NewHighScoreText.enabled = true;
        }
    }
}
