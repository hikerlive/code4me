package com.example.appuidroid;

import java.util.HashSet;
import java.util.Set;

import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.example.appuidroid.sms.Threads;

public class SMSThreadActivity extends BaseActivity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.sms_thread);
		initWidget();
	}
	
	public void initWidget() {
		onClick(R.id.query_sms_threadid, new View.OnClickListener(){
			@Override
			public void onClick(View v) {
				Set<String> recipients = new HashSet<String>();
				recipients.add("+8615855107795");
				
				int thread_id = (int) Threads.getOrCreateThreadId(getApplicationContext(), recipients);
				Log.d("SMSThreadActivity", "thread_id:" + thread_id);
			}
		});
	}

}