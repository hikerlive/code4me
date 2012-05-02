package com.example.baikudroid;

import com.example.baikudroid.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.view.View;

public class MainActivity extends Activity {	
	
	// View
	private Button mLoginButton;
	private Button mContactButton;
	private Button mAboutButton;
	private Button mIntentButton;
	
	protected void onCreate(Bundle saveInstanceState) {
		super.onCreate(saveInstanceState);
		setContentView(R.layout.main);
		
		mLoginButton = (Button) findViewById(R.id.show_login);
		mLoginButton.setOnClickListener(new View.OnClickListener(){
			public void onClick(View v) {
				Intent intent = LoginActivity.createIntent();
				startActivity(intent);
			}
		});
		
		mContactButton = (Button) findViewById(R.id.show_contact);
		mContactButton.setOnClickListener(new View.OnClickListener(){
			public void onClick(View v) {
				Intent intent = ContactActivity.createIntent();
				startActivity(intent);
			}
		});
		
		mAboutButton = (Button) findViewById(R.id.show_about);
		mAboutButton.setOnClickListener(new View.OnClickListener(){
			public void onClick(View v) {
				Intent intent = AboutActivity.createIntent();
				startActivity(intent);
			}
		});
		
		mIntentButton = (Button) findViewById(R.id.show_intent);
		mIntentButton.setOnClickListener(new View.OnClickListener(){
			public void onClick(View v) {
				Intent intent = IntentActivity.createIntent();
				startActivity(intent);
			}
		});
	}
};