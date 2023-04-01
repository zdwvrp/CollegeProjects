using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Projectile : MonoBehaviour
{

    private BoundsCheck bndCheck;
    private Renderer rend;

    [Header("Set Dynamically")]
    public Rigidbody rigid;
    public float birthTime;
    public float x0;
    public bool phaserLeft = false;
    public bool phaserRight = false;

    [SerializeField]                                                        // a
    private WeaponType _type;                                          // b


    // This public property masks the field _type and takes action when it is set
    public WeaponType type
    {                                           // c
        get
        {
            return (_type);
        }
        set
        {
            SetType(value);                                                // c
        }
    }

    void Awake()
    {
        bndCheck = GetComponent<BoundsCheck>();
        rend = GetComponent<Renderer>();                                     // d
        rigid = GetComponent<Rigidbody>();

        birthTime = Time.time;
    }

    void Update()
    {
        if (bndCheck.offUp)
        {
            Destroy(gameObject);
        }

        if( type == WeaponType.phaser )
        {
            Vector3 tempPos = transform.position;
            float age = Time.time - birthTime;
            float theta = Mathf.PI * 2 * age / .33f;
            float sin = Mathf.Sin(theta);
            if( phaserRight )
            {
                sin *= -1;
            }
            tempPos.x = x0 + 2 * sin;
            transform.position = tempPos;
        }
    }

    /// <summary>
    /// Sets the _type private field and colors this projectile to match the
    ///   WeaponDefinition.
    /// </summary>
    /// <param name="eType">The WeaponType to use.</param>
    public void SetType(WeaponType eType)
    {                               // e
        // Set the _type
        _type = eType;
        WeaponDefinition def = Main.GetWeaponDefinition(_type);
        rend.material.color = def.projectileColor;
    }
}
