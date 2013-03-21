package com.example.test;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ChineseWord {
	public static void main(String[] args) {
		String str = "中文fd我是中国人as ";
		int count = hasChineseWord(str);
		System.out.print(str + "共有 " + count + "个\n");
		
		boolean btrue = hasChineseWord2(str);
		System.out.print(str + "有无汉字情况:" + btrue + "\n");
		
		String str1 = "I am chinese";
		int count1 = hasChineseWord(str1);
		System.out.print(str1 + "共有 " + count1 + "个\n");
		
		boolean btrue1 = hasChineseWord2(str1);
		System.out.print(str1 + "有无汉字情况:" + btrue1 + "\n");
	}
	
	private static int hasChineseWord(String str) {
		int count = 0;
		String regEx = "[\\u4e00-\\u9fa5]";
		Pattern p = Pattern.compile(regEx);
		Matcher m = p.matcher(str);
		while (m.find()) {
			for (int i=0; i<=m.groupCount(); i++) {
				count = count + 1;
			}
		}
		return count;
	}
	
	private static boolean hasChineseWord2(String str) {
		System.out.print(str + ", length:" + str.length() + ", bytes length:" + str.getBytes().length);
		if (str.getBytes().length == str.length()) {
			return false;
		}
		return true;
	}
}