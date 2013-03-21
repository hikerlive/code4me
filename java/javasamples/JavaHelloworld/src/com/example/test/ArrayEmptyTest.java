package com.example.test;

import java.util.ArrayList;

public class ArrayEmptyTest {
	public static void main(String[] args) {
		ArrayList<String> temp = null;
		
		if ((temp != null) && temp.size() > 0) {
			System.out.print("is not empty");
		}
		
		if (false || isOk()) {
			System.out.print("test ||");
		}
	}
	
	public static boolean isOk() {
		System.out.print("is ok");
		return true;
	}
}