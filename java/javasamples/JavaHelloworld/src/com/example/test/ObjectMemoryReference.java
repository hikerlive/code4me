package com.example.test;

import java.util.ArrayList;
import java.util.List;

public class ObjectMemoryReference {
	public static void main(String[] args) {
		List<Item> items = new ArrayList<Item>();
		Item first = null;
		for (int i=0; i<10; i++) {
			String a = Integer.toString(i);
			String b = Integer.toString(i+1);
			first =  new Item(a, b);
			items.add(first);
		}
		for (Item item : items) {
			item = null;
		}
		items.clear();
		items = null;
		System.gc();
		
		if (first != null) {
			System.out.print(first.toString());
		}
	}
}