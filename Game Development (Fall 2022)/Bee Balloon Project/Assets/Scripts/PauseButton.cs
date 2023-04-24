using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PauseButton : MonoBehaviour
{
    public void Pause()
    {
        Time.timeScale = 0;
        GameManager.S.isTimerActive = false;
    }

    public void UnPause()
    {
        Time.timeScale = 1;
        GameManager.S.isTimerActive = true;
    }
}
