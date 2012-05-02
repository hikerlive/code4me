package com.example.baikudroid.ui.base;

import com.example.baikudroid.data.Tweet;
import com.example.baikudroid.ui.module.TweetAdapter;

import android.os.Bundle;
import android.widget.ListView;
import android.util.Log;

public abstract class BaikuListBaseActivity extends BaseActivity {
	private static final String TAG = "BaikuListBaseActivity";
	@Override
	protected boolean _onCreate(Bundle savedInstanceState) {
		Log.d(TAG, "_onCreate");
		if (super._onCreate(savedInstanceState)){
			setContentView(getLayoutId());
			setupState();
			return true;
		}
		return false;
	}
	
	abstract protected int getLayoutId();
	abstract protected ListView getTweetList();
	abstract protected TweetAdapter getTweetAdapter();
	abstract protected void setupState();
	abstract protected void updateTweet(Tweet tweet);
	
	protected void draw() {
		Log.i(TAG, "draw()");
		getTweetAdapter().refresh();
	}
	
	protected void goTop() {
		getTweetList().setSelection(1);
	}
	
	protected void adapterRefresh() {
		getTweetAdapter().refresh();
	}
}