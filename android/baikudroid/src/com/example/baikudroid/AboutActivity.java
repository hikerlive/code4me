package com.example.baikudroid;

import com.example.baikudroid.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class AboutActivity extends Activity {
	
	private static final String LAUNCH_ACTION = "com.example.baikudroid.ABOUT";
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.about);
	}
	
	public static Intent createIntent() {
		Intent intent = new Intent(LAUNCH_ACTION);
		intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		return intent;
	}
}