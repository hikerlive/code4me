package com.example.test;

public class ObjectReference {
	
	public class Object {
		public int a;
		public int b;
		public Object() {
			
		}
	}
	
	public void getString(String str) {
		System.out.print(str);
		str = new String("google");
		System.out.print(str);
	}
	
	public void getString2(String abc) {
		System.out.print(abc);
		abc = "helloworld";
		System.out.print(abc);
	}
	
	public void setObject(Object obj) {
		obj.a = 3;
		obj.b = 4;
	}
	
	public static void main(String[] args) {
		ObjectReference ref = new ObjectReference();
		String str = new String("123");
		ref.getString(str);
		System.out.print(str); // 打印结果仍是123
		
		String abc="abc";
		ref.getString2(abc);
		System.out.print(abc); // 打印结果是abc
		
		ObjectReference.Object obj = ref.new Object();
		obj.a = 1;
		obj.b = 2;
		System.out.print("\n" + obj.a + "," + obj.b); // 打印结果是：1,2
		ref.setObject(obj);
		System.out.print("\n" + obj.a + "," + obj.b); // 打印结果是: 3,4
	}
}