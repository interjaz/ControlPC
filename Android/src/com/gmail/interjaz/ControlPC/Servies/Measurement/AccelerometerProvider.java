package com.gmail.interjaz.ControlPC.Servies.Measurement;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class AccelerometerProvider implements SensorEventListener {

	public static long timestamp;
	public static float x;
	public static float y;
	public static float z;
	
	private static SensorManager sSensorManager;

	public void open(Context context) {

		// Accelerometer
		sSensorManager = (SensorManager)context.getSystemService(Context.SENSOR_SERVICE);
		Sensor accelerometer = sSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		sSensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_GAME);

	}

	public void close() {

		// Accelerometer
		if(sSensorManager != null) {
			sSensorManager.unregisterListener(this);
		}
	}

	public void onAccuracyChanged(Sensor arg0, int arg1) {}

	public void onSensorChanged(SensorEvent s) {
		
		if(s.timestamp != 0) {
			timestamp = s.timestamp;
			x = s.values[0];
			y = s.values[1];
			z = s.values[2];
		}
	}

}
