package com.example.appuidroid;

import android.app.Activity;
import android.os.Bundle;

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