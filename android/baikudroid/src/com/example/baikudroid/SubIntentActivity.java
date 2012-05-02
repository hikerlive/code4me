package com.example.baikudroid;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class SubIntentActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		TextView tv = new TextView(this);
		tv.setText("�����Զ���action-com.example.baikudroid.INTENTACTION");
		setContentView(tv);
	}
}