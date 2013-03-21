package com.example.test;

public class Item{
	public String a;
	public String b;
	public Item(String a, String b) {
		this.a = a;
		this.b = b;
	}
	
	public String toString() {
		return "item ["
			 + "a=" + a
			 + "b=" + b
			 + "]";
	}
};