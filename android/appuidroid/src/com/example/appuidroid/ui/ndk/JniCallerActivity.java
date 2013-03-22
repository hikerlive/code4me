package com.example.appuidroid.ui.ndk;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

/**
 * 演示说明:
 * 	1.演示调用者发起调用，同时在JNI中回调到调用者代码。
 *  2.即调用者->被调用者->调用者。
 */
public class JniCallerActivity extends Activity{	
	public void onCreate(Bundle savedInstanceStated) {
		super.onCreate(savedInstanceStated);
		Log.d("jni", stringFromJNI());
	}
	
	public String messageMe(String text) {
		System.out.println(text);
		return String.format("[messageMe]%s", text);
	}
	
	public void messageMe2(String text) {
		System.out.print(text);
	}
	
	/**
	 * A native method that is implemented by the 
	 * 'hello-jni' native library, which is packaged
	 * with the application
	 */
	public native String stringFromJNI();
	
	static {
		System.load("hellojni");
	}
}
