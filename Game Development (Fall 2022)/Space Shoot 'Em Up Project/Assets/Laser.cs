using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Laser : MonoBehaviour
{
    GameObject tip;
    LineRenderer line;
    public Material[] materials;
    // Start is called before the first frame update
    void Start()
    {
        tip = transform.Find("Tip").gameObject;
        line = tip.GetComponent<LineRenderer>();
        line.enabled = false;
    }

    // Update is called once per frame
    void Update()
    {
        if( Input.GetButton("Fire1"))
        {
            line.enabled = true;
            Ray ray = new Ray(tip.transform.position, transform.up);
            RaycastHit hit;

            if( Physics.Raycast(ray, out hit, 100))
            {
                line.SetPosition(1, hit.point);
                ShowDamage(hit.collider.gameObject);
            } else
            {
                line.SetPosition(1, ray.GetPoint(100));
            }
        } else
        {
            line.enabled = false;
        }
    }

    void ShowDamage(GameObject go)
    {
        materials = Utils.GetAllMaterials(go);
        foreach(Material m in materials)
        {
            m.color = Color.red;
        }
    }
}
