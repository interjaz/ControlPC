package com.gmail.interjaz.ControlPC.Servies;

import java.io.IOException;
import java.util.UUID;

import com.gmail.interjaz.ControlPC.Servies.Bluetooth.BluetoothMessage;
import com.gmail.interjaz.ControlPC.Servies.Bluetooth.IBluetoothReceiver;
import com.gmail.interjaz.ControlPC.Servies.Measurement.AccelerometerProvider;

import android.app.Service;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Looper;
import android.widget.Toast;

// Service must be started with enabled Bluetooth
public class BluetoothManagerService extends Service {
	
	public final static UUID BT_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	public final static String MSG_TYPE = "MSG_TYPE";
	public final static String DATA = "DATA";
	public final static int CONNECT = 0x00;
	public final static int SEND_DATA = 0x01;
	
	private static IBluetoothReceiver sReceiver;
	
	public static Boolean isDeviceSet = false;
	
	private static BluetoothSocket sSocket;
	static Context sContext;
	
	@Override
	public IBinder onBind(Intent arg0) {
		return null;
	}

	@Override
	public void onCreate() {
		isDeviceSet = false;
	}

	
	
	@Override
	public void onDestroy() {
		if(isDeviceSet) {
			try {
				sSocket.close();
			} catch (IOException e) {
				// Shit happens
			}			
		}
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {

		sContext = this;
		Bundle extra = intent.getExtras();
		
		if(extra.getInt(MSG_TYPE) == CONNECT) {
			
			// Connect to device
			BluetoothDevice device = extra.getParcelable(DATA);	
			
			try {
				sSocket = device.createRfcommSocketToServiceRecord(BT_UUID);
				sSocket.connect(); 
				isDeviceSet = true;
				Toast.makeText(this, "Connected", Toast.LENGTH_SHORT).show();
			} catch (IOException e) {
				isDeviceSet = false;
			}
			
			// Start new handling Thread
			new Thread(ServiceThread).start();
		}
		
		return START_NOT_STICKY;
	}
	
	private final static Runnable ServiceThread = new Runnable() {

		private AccelerometerProvider mAccelerometer;
		private BluetoothMessage mMessage;
		
		public void run() {
			Looper.prepare();
			
			mAccelerometer = new AccelerometerProvider();
			mAccelerometer.open(sContext);
			
			mMessage = new BluetoothMessage();
			
			while(true) {
				
				mMessage.create();
				mMessage.SetAcc(mAccelerometer.timestamp, mAccelerometer.x, mAccelerometer.y, mAccelerometer.z);
				mMessage.finish();
				
				try {
					sSocket.getOutputStream().write(mMessage.getBytes());
				} catch (IOException e) {
					
				}
				
			}
			
		}
		
	
	};

}
