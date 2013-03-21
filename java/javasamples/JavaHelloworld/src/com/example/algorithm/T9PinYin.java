package com.example.algorithm;

public class T9PinYin {
	public boolean shuoxie;
	public String str;
	public T9PinYin(String str, boolean shuoxie) {
		this.shuoxie = shuoxie;
		this.str = str;
	}
	
	public String toString() {
		return "item=["
			+ "str=" + str
			+ "]";
	}
}