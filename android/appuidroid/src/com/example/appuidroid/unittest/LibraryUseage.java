package com.example.appuidroid.unittest;

import android.util.Log;

import com.channelsoft.baiku4android.Status;

public class LibraryUseage {
	public static final String TAG = "unittest";
	public LibraryUseage() {
		Status status = new Status();
		Log.d(TAG, status.getClass().getName());
	}
}