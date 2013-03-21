package com.example.test;

import java.util.Date;

import com.example.data.JavaHelloworldData;

public class Timestamp {
	public Timestamp() {
		
	}
	
	public static void main(String[] argc) {
		JavaHelloworldData data = new JavaHelloworldData();
		int type = data.getType();
		System.out.print("a call from jar package type:" + type);
		
		String hello = data.getString();
		System.out.print("a call from jar package string:" + hello);
		
		String msgid=genMessageId("3662e693058da6190d17adb9e621ce91", new Date().getTime());
		System.out.print("msgid:" + msgid);
		
		String strtime = msgid.substring(0, 16);
		long timestamp = Long.parseLong(strtime, 16);
		System.out.print("\n"+timestamp);
	} 
	
	public static String genMessageId(String userid, long number) {
			return String.format("%1$016X%2$s", number, userid);
	}
}
