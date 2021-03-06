package com.channelsoft.baikudialer.utils;

public class Hz2PyUtil {
	
	/*
	 * A native method that is implement by the
	 * 'pinyin' native library, which is packaged
	 * with this application.
	 */
	public final static native String hz2py(String str);
	
	public final static native String getSortkey(String str);
	
	public final static native String getSortkeyCodes(String str);
	
	/*
	 * this is used to load the 'pinyin' library on application
	 * startup. The library has already been unpacked into
	 * /data/data/com.channelsoft.baikudialer/libpinyin.so at
	 * installation time by the package manager.
	 */
	static {
		System.loadLibrary("hz2py");
	}
	
}