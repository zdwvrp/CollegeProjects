using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FallingBox : MonoBehaviour
{
    [Header("Set in Inspector")]
    public float baseSpeed = 1f;
    public int minHealth = 1;
    public int maxHealth = 10;

    [Header("Set Dynamically")]
    public int score;      // Points earned for destroying this
    public int health;     // Box Health
    public float speed;
    public Text healthText;
    public bool isColliding = false;
    public Vector3 pos
    {
        get
        {
            return (this.transform.position);
        }
        set
        {
            this.transform.position = value;
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        speed = baseSpeed * Main.S.boxSpeedMultiplier;
        healthText = gameObject.GetComponentInChildren<Text>();

        health = Random.Range(minHealth, maxHealth);
        score = health;
    }

    public virtual void Move()
    {
        Vector3 tempPos = pos;
        tempPos.y -= speed * Time.deltaTime;
        pos = tempPos;
    }

    // Decrements the health of the box when hit by a cannon ball.
    // Returns health value.
    public int HitByCannonBall(int damage)
    {
        health -= damage;
        return health;
    }

    // param addOrSubtract - true if adding score, false if subtracting score
    public void RemoveBox(bool addOrSubtract)
    {
        Main.S.DisplayScore(pos, score, addOrSubtract);
        Destroy(gameObject);
    }
    // Update is called once per frame
    void Update()
    {
        isColliding = false;
        Move();
        healthText.text = "" + health;

        if (health == 0)
        {
            Main.S.AddScore(score);
            RemoveBox(true);
        }

        if (health < 0)
        {
            Main.S.SubtractScore(score);
            RemoveBox(false);
        }

        if( Main.S.timer <= 0f || Main.S.gameMode != GameMode.playing )
        {
            Destroy(gameObject);
        }
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (!isColliding)
        {
            isColliding = true;
            GameObject other = collision.gameObject;
            if (other.tag == "CannonBall1")
            {
                Destroy(other);
                int remainingHealth = HitByCannonBall(1);
                Debug.Log("Box Health is now " + remainingHealth);
            }
            if(other.tag == "CannonBall2")
            {
                Destroy(other);
                int remainingHealth = HitByCannonBall(2);
                Debug.Log("Box Health is now " + remainingHealth);
            }
            if(other.tag == "CannonBall3")
            {
                Destroy(other);
                int remainingHealth = HitByCannonBall(3);
                Debug.Log("Box Health is now " + remainingHealth);
            }

            if (other.tag == "Ground")
            {
                Main.S.SubtractScore(score);
                RemoveBox(false);
            }
        }
    }
}
