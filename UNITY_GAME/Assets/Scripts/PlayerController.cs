using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System.IO.Ports;


public class PlayerController : MonoBehaviour
{

    public Rigidbody rb;
    public float moveSpeed;

    private Vector2 moveInput;


    //SerialPort data_stream = new SerialPort("COM3", 19200); //PC
    SerialPort data_stream = new SerialPort("/dev/cu.usbmodem14401", 19200); //MAC

    public string receivedstring;
    public string[] serialData;


    // Start is called before the first frame update
    void Start()
    {
        try {
            data_stream.Open();
            data_stream.ReadTimeout = 20;

        }
        catch {
            Debug.Log("Could not open serial port");
        } 
    }

    // Update is called once per frame
    void Update()
    {
        string dataString = readSerialData();
        serialData = dataString.Split(',');

        moveInput.x = float.Parse(serialData[0]);
        moveInput.y = float.Parse(serialData[1]);
        moveInput.Normalize();
        Debug.Log(moveInput.x);
        Debug.Log(moveInput.y);
        rb.velocity = new Vector3(moveInput.x * moveSpeed, rb.velocity.y, moveInput.y * moveSpeed);
    }

    public string readSerialData()
    {
        receivedstring = data_stream.ReadLine();
        //Debug.Log(receivedstring);


        return receivedstring;
    }

    private void OnTriggerStay(Collider other)
    {
        if(other.gameObject.tag == "CharacterHome")
        {
            if(serialData[3] == "1"){
                SceneManager.LoadScene(1);
            }
        }
    }
}
