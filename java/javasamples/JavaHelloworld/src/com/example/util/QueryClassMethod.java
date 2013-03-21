package com.example.util;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class QueryClassMethod {
	public static int hello() {
		System.out.print("hello");
		return 1;
	}
	
	public static int hello2(ArrayList<Integer> abc) {
		System.out.print("hello2");
		return 2;
	}
	
	public static void main(String[] args) {
		try {
			ArrayList<Integer> abc = new ArrayList<Integer>();
			System.out.print(Integer.class);
			Method method = QueryClassMethod.class.getMethod("hello2", new Class[]{abc.getClass()});
			Integer ret = (Integer) method.invoke(null, abc);
			System.out.print(ret);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}