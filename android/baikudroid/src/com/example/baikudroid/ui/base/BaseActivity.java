package com.example.baikudroid.ui.base;

import com.example.baikudroid.BaikuApplication;
import com.example.baikudroid.db.BaikuDatabase;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;

public class BaseActivity extends Activity {
	private static final String TAG = "BaseActivity";
	protected SharedPreferences mPreferences;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.d(TAG, "onCreate");
		super.onCreate(savedInstanceState);
		_onCreate(savedInstanceState);
	}
	
	protected boolean _onCreate(Bundle savedInstanceState){
		Log.d(TAG, "_onCreate");
		return true;
	}
	
	public BaikuDatabase getDb() {
		return BaikuApplication.mDb;
	}
	
	/*
	public BaikuApi getApi() {
		return BaikuApplication.mApi;
	}
	
	public SharedPreferences getPreferences() {
		return mPreferences;
	}
	*/
}