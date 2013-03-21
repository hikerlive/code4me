package com.channelsoft.baikudialer.utils;

import android.content.res.AssetManager;

public class AreaCodeUtil {
	
	public final static native String getAreaCodeText(AssetManager asset, String number);
	
	static {
		System.load("areacode");
	}
}