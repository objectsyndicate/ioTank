package com.objectsyndicate.iotankemulator

import android.annotation.SuppressLint
import android.app.Activity
import android.app.job.JobInfo
import android.app.job.JobScheduler
import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.graphics.Color
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import android.os.AsyncTask
import android.os.Bundle
import android.os.Handler
import android.os.Message
import android.os.Messenger
import android.support.design.widget.Snackbar
import android.text.TextUtils
import android.view.View
import android.view.WindowManager
import android.widget.CompoundButton
import android.widget.EditText
import android.widget.Switch
import android.widget.ToggleButton

import java.io.BufferedReader
import java.io.DataOutputStream
import java.io.InputStream
import java.io.InputStreamReader
import java.math.BigDecimal
import java.net.URL

import javax.net.ssl.HttpsURLConnection


class MainActivity : Activity(), SensorEventListener {

    internal var sensorsOn: Boolean? = true
    internal var Fahrenheit: Boolean? = true
    internal var Endp: String = ""
    internal var Host: String = ""
    internal var Token: String = ""
    internal var Temp1: String? = null
    internal var Temp2: String? = null
    internal var H: String? = null
    internal var UVI: String? = null
    internal var L: String? = null

    internal var sendEveryX: Boolean? = false

    internal var myServiceComponent: ComponentName? = null
    lateinit var myService: EveryXService

    internal var myHandler: Handler = @SuppressLint("HandlerLeak")
    object : Handler() {
        override fun handleMessage(msg: Message) {
            myService = msg.obj as EveryXService
            myService.setUICallback(this@MainActivity)

        }
    }

    private var mSensorManager: SensorManager? = null
    private var mTemperature: Sensor? = null
    private var mLight: Sensor? = null
    private var mHumidity: Sensor? = null

    public override fun onCreate(savedInstanceState: Bundle?) {

        //-------------------------------------------------------------------------------------------

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        myServiceComponent = ComponentName(this, EveryXService::class.java)
        val myServiceIntent = Intent(this, EveryXService::class.java)
        myServiceIntent.putExtra("messenger", Messenger(myHandler))
        startService(myServiceIntent)


        // Get an instance of the sensor service, and use that to get an instance of
        // a particular sensor.
        mSensorManager = getSystemService(Context.SENSOR_SERVICE) as SensorManager
        mTemperature = mSensorManager!!.getDefaultSensor(Sensor.TYPE_AMBIENT_TEMPERATURE)
        mLight = mSensorManager!!.getDefaultSensor(Sensor.TYPE_LIGHT)
        mHumidity = mSensorManager!!.getDefaultSensor(Sensor.TYPE_RELATIVE_HUMIDITY)

        val switchButton: Switch = findViewById(R.id.OnBoard) as Switch
        val switchButton2: Switch = findViewById(R.id.EveryFive) as Switch
        val switchButton3: ToggleButton = findViewById(R.id.toggleUnit) as ToggleButton

        switchButton.isChecked = true
        switchButton.setOnCheckedChangeListener { compoundButton, bChecked ->
            if (bChecked) {
                sensorsOn = true

            } else {
                sensorsOn = false

            }
        }

        if (switchButton.isChecked) {
            sensorsOn = true

        } else {
            sensorsOn = false

        }

        //-------------------------------------------------------------------------------------------

        switchButton2.isChecked = false
        switchButton2.setOnCheckedChangeListener { compoundButton, bChecked ->
            if (bChecked) {


                val lp = window.attributes
                lp.screenBrightness = 0f
                window.attributes = lp

                val endp = findViewById(R.id.editEndpoint) as EditText
                Endp = endp.text.toString()
                EveryXService.setEndp(Endp)

                val host = findViewById(R.id.editHost) as EditText
                Host = host.text.toString()
                EveryXService.setHost(Host)

                val token = findViewById(R.id.editToken) as EditText
                Token = token.text.toString()
                EveryXService.setToken(Token)

                val builder = JobInfo.Builder(0, myServiceComponent)

                builder.setPeriodic(900000)

                myService.scheduleJob(builder.build())


            } else {

                val jobScheduler = getSystemService(Context.JOB_SCHEDULER_SERVICE) as JobScheduler
                jobScheduler.cancel(0)


                val lp = window.attributes
                lp.screenBrightness = -1f
                window.attributes = lp
            }
        }

        if (switchButton2.isChecked) {


            sendEveryX = true


        } else {

            sendEveryX = false

        }
        //-------------------------------------------------------------------------------------------


        switchButton3.isChecked = false
        switchButton3.setOnCheckedChangeListener { compoundButton, bChecked ->
            if (bChecked) {
                Fahrenheit = true
            } else {
                Fahrenheit = false

            }
        }

        if (switchButton3.isChecked) {

            Fahrenheit = true

        } else {
            Fahrenheit = false
        }


    } //------------------------------ONCREATEEND-----------------------------

    fun stop() {
        this.mSensorManager!!.unregisterListener(this)
    }

    override fun onAccuracyChanged(sensor: Sensor, accuracy: Int) {
        // Do something here if sensor accuracy changes.
    }

    override fun onSensorChanged(event: SensorEvent) {
        val sensor = event.sensor

        if (sensorsOn!!) {

            if (sensor.type == Sensor.TYPE_LIGHT) {
                lux = event.values[0]
            } else if (sensor.type == Sensor.TYPE_RELATIVE_HUMIDITY) {
                humidity = event.values[0]
            } else if (sensor.type == Sensor.TYPE_AMBIENT_TEMPERATURE) {

                if (Fahrenheit!!) {
                    temp = event.values[0] * 9 / 5 + 32
                } else {
                    temp = event.values[0]

                }

            }

            val editText = findViewById(R.id.editTemp2) as EditText
            editText.setText(java.lang.Float.toString(temp))

            val editLux = findViewById(R.id.editLux) as EditText
            editLux.setText(java.lang.Float.toString(lux))

            val editHumidity = findViewById(R.id.editHumidity) as EditText
            editHumidity.setText(java.lang.Float.toString(humidity))
        }
    }

    override fun onResume() {
        // Register a listener for the sensor.
        super.onResume()
        mSensorManager!!.registerListener(this, mLight, SensorManager.SENSOR_DELAY_NORMAL)
        mSensorManager!!.registerListener(this, mTemperature, SensorManager.SENSOR_DELAY_NORMAL)
        mSensorManager!!.registerListener(this, mHumidity, SensorManager.SENSOR_DELAY_NORMAL)

    }

    override fun onPause() {
        // Be sure to unregister the sensor when the activity pauses.
        super.onPause()
        mSensorManager!!.unregisterListener(this)
    }

    fun sendOnClick(v: View) {

        try {

            val endp = findViewById(R.id.editEndpoint) as EditText
            Endp = endp.text.toString()
            if (TextUtils.isEmpty(Endp)) {
                endp.error = "Enter URI Endpoint"
                return
            }

            val host = findViewById(R.id.editHost) as EditText
            Host = host.text.toString()
            if (TextUtils.isEmpty(Endp)) {
                host.error = "Enter Host Domain"
                return
            }

            val token = findViewById(R.id.editToken) as EditText
            Token = token.text.toString()
            if (TextUtils.isEmpty(Token)) {
                token.error = "Enter Authentication Token"
                return
            }

            val t1 = findViewById(R.id.editTemp1) as EditText
            if (Fahrenheit!!) {
                val f = java.lang.Float.valueOf(t1.text.toString())!!
                val i = (f - 32) * 5 / 9
                val bd: BigDecimal
                bd = round(i, 2)
                Temp1 = bd.toString()
            } else {
                Temp1 = t1.text.toString()
            }
            if (TextUtils.isEmpty(Temp1)) {
                t1.error = "Enter Temperature"
                return
            }
            // If F is enabled convert the value to F


            val t2 = findViewById(R.id.editTemp2) as EditText
            if (Fahrenheit!!) {
                val f = java.lang.Float.valueOf(t2.text.toString())!!
                val i = (f - 32) * 5 / 9
                val bd: BigDecimal
                bd = round(i, 0)
                Temp2 = bd.toString()
            } else {
                Temp2 = t2.text.toString()
            }
            if (TextUtils.isEmpty(Temp2)) {
                t2.error = "Enter Temperature"
                return
            }


            val h = findViewById(R.id.editHumidity) as EditText
            H = h.text.toString()
            if (TextUtils.isEmpty(H)) {
                h.error = "Enter Temperature"
                return
            }


            val uvi = findViewById(R.id.editUVI) as EditText
            UVI = uvi.text.toString()
            if (TextUtils.isEmpty(UVI)) {
                uvi.error = "Enter UV Index"
                return
            }


            val lux = findViewById(R.id.editLux) as EditText
            L = lux.text.toString()
            if (TextUtils.isEmpty(L)) {
                lux.error = "Enter Lux"
                return
            }

            val urlString = "https://" + Host + Endp



            val sendURL = URL(urlString)
            DownloadFilesTask().execute(sendURL)
            println(sendURL)

        } catch (e: Exception) {
            println(e)
        }

    }

    private inner class DownloadFilesTask : AsyncTask<URL, Int, Long>() {

        override fun doInBackground(vararg urls: URL): Long? {
            val totalSize: Long = 0
            System.out.println("here")
            try {

                val url = urls[0]
                val conn = url.openConnection() as HttpsURLConnection
                //conn.setSSLSocketFactory(PinnedPublicKeySocketFactory.createSocketFactory());
                conn.doOutput = true
                conn.doInput = true
                conn.setRequestProperty("Content-Type", "application/json")
                conn.setRequestProperty("Authorization", "Token " + Token)

                conn.requestMethod = "POST"
                conn.connectTimeout = 1 * 60 * 1000

                val jsonRequest = "{\"t1\":$Temp1, \"t2\": $Temp2, \"h\":$H, \"uv\": $UVI, \"l\":$L}"
                val out = DataOutputStream(conn.outputStream)
                out.writeBytes(jsonRequest)
                conn.connect()
                System.out.println(conn.getResponseCode())
                System.out.println(jsonRequest)

                if (conn.responseCode == 201) {
                    val input = conn.inputStream
                    val reader = BufferedReader(InputStreamReader(input))
                    val line: String = reader.readLine()

                        System.out.println(line)
                        Snackbar.make(findViewById(android.R.id.content), "Posted at " + line, Snackbar.LENGTH_LONG)
                                .setActionTextColor(Color.RED)
                                .show()

                } else {
                    Snackbar.make(findViewById(android.R.id.content), "HTTP Status Code " + conn.responseCode, Snackbar.LENGTH_LONG)
                            .setActionTextColor(Color.RED)
                            .show()
                }


                out.flush()
                out.close()


            } catch (e: Exception) {
                println(e)
            }

            return totalSize
        }

    }

    companion object {
        var lux: Float = 0.toFloat()
        var humidity: Float = 0.toFloat()
        var temp: Float = 0.toFloat()


        //-------------------------------------------------------------------------------------------

        fun round(d: Float, decimalPlace: Int): BigDecimal {
            var bd = BigDecimal(java.lang.Float.toString(d))
            bd = bd.setScale(decimalPlace, BigDecimal.ROUND_HALF_UP)
            return bd
        }
    }


}


