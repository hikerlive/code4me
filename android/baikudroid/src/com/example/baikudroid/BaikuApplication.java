package com.example.baikudroid;

import com.example.baikudroid.db.BaikuDatabase;

import android.app.Application;
import android.util.Log;

public class BaikuApplication extends Application {
	public static final String TAG = "BaikuApplication";
	public static BaikuDatabase mDb;
	
	@Override
	public void onCreate() {
		Log.d(TAG, "onCreate");
		super.onCreate();
		mDb = BaikuDatabase.getInstance(this);
	}
	
	@Override
	public void onTerminate() {
		Log.d(TAG, "onTerminate");
		mDb.close();
		super.onTerminate();
	}
}