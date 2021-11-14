using System.Collections;
using UnityEngine;
using System.IO.Ports;

public class moveCube : MonoBehaviour
{
    SerialPort data_stream = new SerialPort("COM3", 19200);
    public string receivedstring;
    public GameObject test_data;
    public Rigidbody rb;
    public float sensitivity = 0.01f;

    private Renderer render;
    private Color oldColor;

    public string[] datas;

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

        render = GetComponent<Renderer>();
        oldColor = render.material.color;

        //InvokeRepeating("readSerialData", 0f, 0.01f);
    }

    // Update is called once per frame
    void Update()
    {
        string dataString = readSerialData();

        string[] serialData = dataString.Split(',');

        transform.Translate(float.Parse(serialData[0])*sensitivity * Time.deltaTime, float.Parse(serialData[1])*sensitivity * Time.deltaTime, 0);
        Debug.Log(serialData[2] + ' ' + serialData[3]);

        if(serialData[2] == "1"){
            render.material.color = Color.blue;
        }else if(serialData[3] == "1"){
            render.material.color = Color.green;
        } else if(serialData[4] == "1") {
            render.material.color = Color.red;
        }else {
            render.material.color = oldColor;
        }

        // if(serialData[3] == "1" && serialData[2] == "0"){
        //     render.material.color = Color.green;
        // }else {
        //     render.material.color = oldColor;
        // }

        // if(serialData[2] == "1" && serialData[3] == "1"){
        //     render.material.color = Color.red;
        // }else {
        //     render.material.color = oldColor;
        // }
        
        //transform.Translate(float.Parse(datas[0])*sensitivity * Time.deltaTime, 0, float.Parse(datas[1])*sensitivity * Time.deltaTime);
        //transform.Rotate(float.Parse(datas[3]), float.Parse(datas[2]), 0);


    }

    public string readSerialData()
    {
        receivedstring = data_stream.ReadLine();
        //Debug.Log(receivedstring);


        return receivedstring;
    }
}