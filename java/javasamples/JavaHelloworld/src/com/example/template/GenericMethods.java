package com.example.template;

public class GenericMethods {
	public <T> void print(T x) {
		System.out.print(x.getClass().getName());
	}
	
	public static void main(String[] args) {
		GenericMethods gm = new GenericMethods();
		gm.print(new OrderBean("aaa"));
	}
}