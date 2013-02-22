package com.example.appuidroid.utils;

public class HelloJni {
	/**
	 * A native method that is implemented by the 
	 * 'hello-jni' native library, which is packaged
	 * with the application
	 */
	public native String stringFromJNI();
	
	static {
		System.loadLibrary("hellojni");
	}
}
