package com.example.appuidroid;

import android.app.Activity;
import android.content.Context;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.widget.ListView;
import android.widget.TextView;

/**
 * 界面效果：
 * 1.实现类似于iPhone联系人拼音首字母的快速定位栏，点击并可以显示出按的是那个字。
 * 2.定制View来实现的，截获View上面的按上和滑动事件。
 * 3.如何计算点击是那个字符，通过触摸高度与定位栏高度来实现，字符所占高是等比的。
 */
public class LetterListActivity extends Activity implements LetterListViewListener{
	
	private static final String TAG = "LetterListActivity";
	private LetterListView mLetterListView;
	private ListView mListView;
	private Handler mHandler;
	private OverlayThread mOverlayThread;
	private TextView mOverlay;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.contact_selector);
		
		mListView = (ListView) findViewById(R.id.list_view);
		View list_view = findViewById(R.id.letter_list_view);
		if (null != list_view) {
			
		}
		
		/**
			问题：为什么返回的会是空的呢？，这个控件id是自定义的View。
		 	因为在自定义的LetterListView控件中的构造函数写成如下方式：
		 	LetterListView(Context context, AttributeSet attrSet) {
		    	super(context); // 而应该是super(context, attrSet);
		 	}
		 */
		mLetterListView = (LetterListView) findViewById(R.id.letter_list_view);
		if (null != mLetterListView) {
			mLetterListView.setListener(this); 
		}
		initOverlay();
		
		mHandler = new Handler();
		mOverlayThread = new OverlayThread();
	}
	
	public void onDestroy() {
		destroyOverlay();
		super.onDestroy();
	}
	
	public void onTouchingLetterChanged(final String s) {
		Log.d(TAG, "click character:" + s);
		mOverlay.setText(s);
		mOverlay.setVisibility(View.VISIBLE);
		mHandler.removeCallbacks(mOverlayThread);
		// 延迟一秒后执行，让overlay为不可见
		mHandler.postDelayed(mOverlayThread, 1500);
	}
	
	// 设置overlay不可见
	private class OverlayThread implements Runnable {
		@Override
		public void run() {
			mOverlay.setVisibility(View.GONE);
		}
	}
	
	// 初始化汉语拼音首字母弹出提示框
	private void initOverlay() {
		LayoutInflater inflater = LayoutInflater.from(this);
		mOverlay = (TextView)inflater.inflate(R.layout.overlay, null);
		mOverlay.setVisibility(View.INVISIBLE);
		WindowManager.LayoutParams lp = new WindowManager.LayoutParams(
				LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT,
				WindowManager.LayoutParams.TYPE_APPLICATION,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
					| WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
				PixelFormat.TRANSLUCENT);
		WindowManager wm = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
		wm.addView(mOverlay, lp);
	}
	private void destroyOverlay() {
		WindowManager wm = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
		wm.removeView(mOverlay);
		mOverlay = null;
	}
}
