package com.example.baikudroid;

import com.example.baikudroid.data.Tweet;
import com.example.baikudroid.db.ContentTable;
import com.example.baikudroid.ui.base.BaikuCursorBaseActivity;

import android.app.Activity;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;

public class BaikuActivity extends BaikuCursorBaseActivity {
		
	// protected ShakeListener mShaker = null;
	private static final String TAG = "BaikuActivity";
	    
    @Override
    protected boolean _onCreate(Bundle savedInstanceState) {
    	Log.d(TAG, "_onCreate");
    	if (super._onCreate(savedInstanceState)) {
    		return true;
    	}
    	return false;
    }
    
    @Override
    protected Cursor fetchMessages() {
    	return getDb().fetchAllTweets("test_userid", ContentTable.TYPE_HOME);
    }
}