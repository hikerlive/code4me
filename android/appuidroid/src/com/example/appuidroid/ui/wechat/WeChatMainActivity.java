package com.example.appuidroid.ui.wechat;

import android.app.Activity;
import android.os.Bundle;
import android.widget.LinearLayout;
import android.widget.TextView;

public class WeChatMainActivity extends Activity{
	
	private LinearLayout mLinearLayout;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		mLinearLayout = new LinearLayout(this);
		TextView view = new TextView(this);
		view.setText("欢迎使用WeChat");
		mLinearLayout.addView(view);
		
		setContentView(mLinearLayout);
	}
}
