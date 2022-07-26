using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(Player))]
public class PlayerEditor : Editor
{
    private void OnSceneGUI()
    {
        Player player = (Player)target;

        Handles.color = Color.white;
        Handles.DrawWireArc(player.transform.position, Vector3.up, Vector3.forward, 360, player.view);

        for(int i =0; i<player.visibleTargets.Count; i++)
        {
            Handles.DrawLine(player.transform.position, player.visibleTargets[i].position);
        }
    }
}
