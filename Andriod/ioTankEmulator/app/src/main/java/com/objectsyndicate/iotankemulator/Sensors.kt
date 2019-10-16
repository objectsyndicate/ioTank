package com.objectsyndicate.iotankemulator

import android.app.Activity
import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager

class Sensors {
    inner class SensorActivity : Activity(), SensorEventListener {
        private val mSensorManager: SensorManager = getSystemService(Context.SENSOR_SERVICE) as SensorManager
        private val mTemperature: Sensor
        private val mLight: Sensor
        private val mHumidity: Sensor

        init {
            mTemperature = mSensorManager.getDefaultSensor(Sensor.TYPE_AMBIENT_TEMPERATURE)
            mLight = mSensorManager.getDefaultSensor(Sensor.TYPE_LIGHT)
            mHumidity = mSensorManager.getDefaultSensor(Sensor.TYPE_RELATIVE_HUMIDITY)
        }

        override fun onResume() {
            super.onResume()
            mSensorManager.registerListener(this, mLight, SensorManager.SENSOR_DELAY_NORMAL)
            mSensorManager.registerListener(this, mTemperature, SensorManager.SENSOR_DELAY_NORMAL)
            mSensorManager.registerListener(this, mHumidity, SensorManager.SENSOR_DELAY_NORMAL)
        }

        override fun onPause() {
            super.onPause()
            mSensorManager.unregisterListener(this)
        }

        override fun onAccuracyChanged(sensor: Sensor, accuracy: Int) {}

        override fun onSensorChanged(event: SensorEvent) {}
    }

}
