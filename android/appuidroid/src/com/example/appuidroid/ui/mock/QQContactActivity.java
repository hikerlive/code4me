package com.example.appuidroid.ui.mock;

import android.app.Activity;
import android.os.Bundle;

import com.example.appuidroid.R;
import com.example.appuidroid.R.id;
import com.example.appuidroid.R.layout;
import com.tencent.qqphonebook.views.DialPadView;

public class QQContactActivity extends Activity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.qqcontact_dialerpad);
		
		DialPadView dialPadView = (DialPadView) findViewById(R.id.dial_container);
		if (null != dialPadView) {
			
		}
	}
}