package com.example.algorithm;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class PinYinSort {
	
	public static void main(String[] argc) {
		PinYinSort pys = new PinYinSort();
		pys.pinyinCase2();
	}
	
	public void pinyinCase2() {
		class Item {
			public boolean shuoxie;
			public String abc;
			
			public Item(boolean a, String b) {
				shuoxie = a;
				abc = b;
			}
			
			public String toString() {
				return abc;
			}
		}
		
		List<String> array = new ArrayList<String>();
		array.add("YOU");
		array.add("ZHEN");
		array.add("XIONG");

		int index = 0;
		Map<Integer, List<Item>> maps= new HashMap<Integer, List<Item>>();
		for (String one : array) {
			index += 1;
			List<Item> a = new ArrayList<Item>();
			a.add(new Item(true, one.substring(0, 1)));
			if (one.length() > 2) {
				if ('H' == one.charAt(1)) {
					a.add(new Item(true, one.substring(0, 2)));
				}
			}
			a.add(new Item(false, one));
			maps.put(index, a);
		}
		
		int a = 2;
		for (int i=0; i<=array.size()-2; i++) {
			List<Item> ids1 = (List<Item>)maps.get(1);
			List<Item> ids2 = (List<Item>)maps.get(a);
			List<Item> nids = new ArrayList<Item>();
			for (Item str1 : ids1) {
				for (Item str2 : ids2) {
					if (str1.shuoxie) {
						if (str2.shuoxie) {
							nids.add(new Item(true, str1.abc+str2.abc));
						} 
					} else {
						nids.add(new Item((str1.shuoxie || str2.shuoxie) , str1.abc+str2.abc));
					}
				}
			}
			ids1.clear();
			ids1 = null;
			
			ids2.clear();
			ids2 = null;
			
			maps.put(1, nids);
			a = a + 1;
		}
		
		List<Item> ares = (List<Item>)maps.get(1);
		System.out.print(ares);
		ares.clear();
		ares = null;
		maps.clear();
		maps = null;
	}
	
	public void pinyinCase1() {
		List<String> array = new ArrayList<String>();
		array.add("YOU");
		array.add("ZHEN");
		array.add("XIONG");
		
		List<String> pinyins = new ArrayList<String>();
		int pos = -1;
		while (pos < array.size()) {
			String pinyin = "";
			for (int i=0; i<array.size(); i++) {
				String py = array.get(i);
				if (pos == -1) {
					pinyin += py.charAt(0);
				} else {
					if (pos >= i){
						pinyin += py;
					} else {
						pinyin += py.charAt(0);
					}
				}
			}
			pinyins.add(pinyin);
			pos += 1;
		}
		
		System.out.print(pinyins);
	}
}