package com.example.appuidroid.asynchttp;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;

public class AsyncHttpActivity extends Activity{
	protected void onCreate(Bundle savnedInstanceState) {
		super.onCreate(savnedInstanceState);
		makeRequest();
	}
	
	public void makeRequest() {
		AsyncHttpClient client = new AsyncHttpClient();
		client.get("http://www.google.com", new AsyncHttpResponseHandler(){
			@Override
			public  void onSuccess(String resposne) {
				Log.d("test", "get google success");
			}
		});
	}
}