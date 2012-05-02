package com.example.baikudroid;

import com.example.baikudroid.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class LoginActivity extends Activity {
	
	private String mUsername;
	private String mPassword;
	
	private EditText mUsernameEdit;
	private EditText mPasswordEdit;
	private TextView mProgressText;
	private Button mLoginButton;
	
	private static final String LAUNCH_ACTION = "com.example.baikudroid.LOGIN";
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.login);
		
		mUsernameEdit = (EditText) findViewById(R.id.username_edit);
		mPasswordEdit = (EditText) findViewById(R.id.password_edit);
		
		mProgressText = (TextView) findViewById(R.id.progress_text);
		mProgressText.setFreezesText(true);
		
		mLoginButton = (Button) findViewById(R.id.signin_button);
		mLoginButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
			}
		});
	}
	
	public static Intent createIntent() {
		Intent intent = new Intent(LAUNCH_ACTION);
		intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		return intent;
		
	}
}