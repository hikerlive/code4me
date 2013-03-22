package com.example.appuidroid.ui.ndk;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.channelsoft.baikudialer.utils.Hz2PyUtil;

public class PinYinActivity extends Activity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		testCaseHz2py();
	}
	
	private void testCaseHz2py() {
		/*
		String text = "中国123淫";
		String pinyin = Hz2PyUtil.hz2py(text);
		Log.d("jni", "text:" + text + ",pinyin:" + pinyin);
		
		String text1 = "刘@123德华";
		String py1 = Hz2PyUtil.hz2py(text1);
		Log.d("jni", "text1:" + text1 + ",pinyin:" + py1);
		
		String text2 = "hao123";
		String py2 = Hz2PyUtil.hz2py(text2);
		Log.d("jni", "text2:" + text2 + ",pinyin:" + py2);
		
		String text3 = "游 振雄";
		String py3 = Hz2PyUtil.hz2py(text3);
		Log.d("jni", "text3:" + text3 + ",pinyin:" + py3);
		
		String text4 = "gonggai@163.com";
		String py4 = Hz2PyUtil.hz2py(text4);
		Log.d("jni", "text4:" + text4 + ",pinyin:" + py4);
		*/
		// Log.d("jni", stringFromJNI());
		
		String text3 = "游振雄";
		String pinyin = Hz2PyUtil.hz2py(text3);
		String sortkey3 = Hz2PyUtil.getSortkey(text3);
		String codes = Hz2PyUtil.getSortkeyCodes(sortkey3);
		Log.d("jni", "text3:" + text3 + ",pinyin:" + pinyin + ",sortkey3:" + sortkey3 + ",codes:" + codes);
	}
}