package com.example.appuidroid;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class SensorMonitorActivity extends Activity implements SensorEventListener{
	private final static String TAG = "SensorMonitorActivity";
	private WakeLock mWakeLock;
	private float mProximityRange;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.sensor_monitor);
		
		Button btn1 = (Button)findViewById(R.id.button1);
		btn1.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				acquireWakeLock();
				/*
				PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
				pm.goToSleep(SystemClock.uptimeMillis());
				*/
				
			}
		});
		Button btn2 = (Button)findViewById(R.id.button2);
		btn2.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				releaseWakeLock();
				// getWindow().addFlags(WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED);
				// getWindow().addFlags(WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD);
			}
		});
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		Log.d(TAG, "onAccuracyChanged type:");
	}
	
	@Override
	public void onSensorChanged(SensorEvent event) {
		float x = event.values[0];
		Log.d(TAG, "onSensorChanged distance:" + x);
		if (x == mProximityRange) {
		} else {
		}
	}
	
	@Override
	public void onResume() {
		super.onResume();
		SensorManager sm = (SensorManager)getSystemService(Context.SENSOR_SERVICE);
		Sensor sensor = sm.getDefaultSensor(Sensor.TYPE_PROXIMITY);
		mProximityRange = sensor.getMaximumRange();
		sm.registerListener(this, sensor, SensorManager.SENSOR_DELAY_NORMAL);
	}
	
	@Override
	public void onPause() {
		super.onPause();
		SensorManager sm = (SensorManager)getSystemService(Context.SENSOR_SERVICE);
		sm.unregisterListener(this);
	}
	
	private void acquireWakeLock() {
		if (mWakeLock == null) {
			Log.d(TAG, "acquireWakeLock");
			int PROXIMITY_SCREEN_OFF_WAKE_LOCK = 32;
			PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
			mWakeLock = pm.newWakeLock(PROXIMITY_SCREEN_OFF_WAKE_LOCK,
					this.getClass().getCanonicalName());
			mWakeLock.acquire();
		}
	}
	
	private void releaseWakeLock() {
		if (mWakeLock != null) {
			Log.d(TAG, "releaseWakeLock");
			if (mWakeLock.isHeld()) {
				mWakeLock.release();
			}
			mWakeLock = null;
		}
	}
}