package com.objectsyndicate.iotankemulator

import android.app.Service
import android.app.job.JobInfo
import android.app.job.JobParameters
import android.app.job.JobScheduler
import android.app.job.JobService
import android.content.Context
import android.content.Intent
import android.graphics.Color

import android.os.AsyncTask
import android.os.Message
import android.os.Messenger
import android.os.RemoteException
import android.support.design.widget.Snackbar
import android.util.Log

import java.io.BufferedReader
import java.io.DataOutputStream
import java.io.InputStream
import java.io.InputStreamReader
import java.net.URL

import javax.net.ssl.HttpsURLConnection


class EveryXService : JobService() {


    private inner class DownloadFilesTask : AsyncTask<URL, Int, Long>() {
        override fun doInBackground(vararg urls: URL): Long? {
            val totalSize: Long = 0

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

                val jsonRequest = "{\"t1\":0, \"t2\": " + MainActivity.temp + ", \"h\":" + MainActivity.humidity + ", \"uv\": 0, \"l\":" + MainActivity.lux + "}"
                val out = DataOutputStream(conn.outputStream)
                out.writeBytes(jsonRequest)
                conn.connect()
                println(conn.responseCode)
                println(jsonRequest)

                if (conn.responseCode == 201) {
                    val input = conn.inputStream
                    val reader = BufferedReader(InputStreamReader(input))
                    val line: String = reader.readLine()

//

                } else {
//
                }



                out.flush()
                out.close()


            } catch (e: Exception) {
                println(e)
            }

            return totalSize
        }

    }


    // This method is called when the service instance
    // is created
    override fun onCreate() {
        super.onCreate()
        Log.i("MyService", "myService created")

        println("invoked")
    }

    // This method is called when the service instance
    // is destroyed
    override fun onDestroy() {
        super.onDestroy()
        Log.i("MyService", "myService destroyed")
    }

    // This method is called when the scheduled job
    // is started
    override fun onStartJob(params: JobParameters): Boolean {
        Log.i("MyService", "on start job")


        val urlString = "https://" + Host + Endp


        try {
            val sendURL = URL(urlString)
            DownloadFilesTask().execute(sendURL)
        } catch (e: Exception) {
            println(e)
        }



        return true
    }

    // This method is called when the scheduled job
    // is stopped
    override fun onStopJob(params: JobParameters): Boolean {
        Log.i("MyService", "on stop job")
        return true
    }

    internal var myMainActivity: MainActivity? = null

    fun setUICallback(activity: MainActivity) {
        myMainActivity = activity
    }


    // This method is called when the start command
    // is fired
    override fun onStartCommand(intent: Intent, flags: Int,
                                startId: Int): Int {
        val callback = intent.getParcelableExtra<Messenger>("messenger")
        val m = Message.obtain()
        m.what = 2
        m.obj = this
        try {
            callback.send(m)
        } catch (e: RemoteException) {
            Log.e("MyService", "Error passing service obje back to activity.")
        }

        return Service.START_NOT_STICKY
    }

    // Method that schedules the job
    fun scheduleJob(build: JobInfo) {
        Log.i("MyService", "Scheduling job")
        val jobScheduler = getSystemService(Context.JOB_SCHEDULER_SERVICE) as JobScheduler
        jobScheduler.schedule(build)
    }

    companion object {
        internal var Endp: String = ""
        internal var Host: String = ""
        internal var Token: String = ""

        fun setEndp(endp: String) {
            Endp = endp
        }


        fun setHost(host: String) {
            Host = host
        }


        fun setToken(token: String) {
            Token = token
        }


    }


}

