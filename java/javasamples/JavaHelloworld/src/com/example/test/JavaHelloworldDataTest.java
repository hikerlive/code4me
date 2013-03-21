package com.example.test;

import com.example.data.JavaHelloworldData;

public class JavaHelloworldDataTest {
	public JavaHelloworldDataTest() {
		
	}
	
	public static void main(String[] argc) {
		JavaHelloworldData data = new JavaHelloworldData();
		int type = data.getType();
		System.out.print("a call from jar package type:" + type);
		
		String hello = data.getString();
		System.out.print("a call from jar package string:" + hello);
		
	}
}