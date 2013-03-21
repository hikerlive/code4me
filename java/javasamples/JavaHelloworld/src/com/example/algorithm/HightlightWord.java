package com.example.algorithm;

import java.util.regex.Pattern;

import net.sourceforge.pinyin4j.PinyinHelper;
import net.sourceforge.pinyin4j.format.HanyuPinyinCaseType;
import net.sourceforge.pinyin4j.format.HanyuPinyinOutputFormat;
import net.sourceforge.pinyin4j.format.HanyuPinyinToneType;
import net.sourceforge.pinyin4j.format.exception.BadHanyuPinyinOutputFormatCombination;

public class HightlightWord {
	public static final Pattern pattern = Pattern.compile("^[A-Za-z0-9]+$");
	
	public static void main(String[] argc) {
		String name = "龚@盖";
		
		StringBuffer pybf = new StringBuffer();
		char[] arr = name.toCharArray();
		HanyuPinyinOutputFormat defaultFormat = new HanyuPinyinOutputFormat();
		defaultFormat.setCaseType(HanyuPinyinCaseType.LOWERCASE);
		defaultFormat.setToneType(HanyuPinyinToneType.WITHOUT_TONE);

		int pretype = -1;
		String word = "";
		for (int i=0; i<arr.length; i++) {
			char c = arr[i];
			int type = -1;
			if (c > 128) { // 汉字
				type = 1;
				if (pretype == -1) { pretype = 1; }
			} else if (pattern.matcher(Character.toString(c)).matches()){ // 拼音与数字
				type = 2;
				if (pretype == -1) { pretype = 2; }
			} else { // 特殊字符
				type = 3;
				if (pretype == -1) { pretype = 3; }
			}
			
			if (type == 1) {
				if (pretype == 2) {
					pybf.append(strToAscII(word) + "\t");
					word = "";
				}
				else if (pretype == 3) {
					pybf.append(word + "\t");
					word = "";
				}
				try {
					String[] tmp = PinyinHelper.toHanyuPinyinStringArray(c, defaultFormat);
					if (null != tmp) {
						pybf.append(strToAscII(tmp[0]) + "\t");
					}
				} catch (BadHanyuPinyinOutputFormatCombination e) {
					e.printStackTrace();
				}
			} 
			else if (type == 2) {
				if (pretype == 3) {
					pybf.append(word + "\t");
					word = "";
				}
				word += c;
			}
			else if (type == 3) {
				if (pretype == 2) {
					pybf.append(strToAscII(word) + "\t");
					word = "";
				}
				word += c;
			}
			pretype = type;
			if (i == arr.length-1) {
				word = (pretype == 2) ? strToAscII(word) : word;
				pybf.append(strToAscII(word) + "\t");
				word = "";
			}
		}
		String abc = pybf.toString().trim();
		System.out.print(abc);
	}
	
	public static String strToAscII(String pinyinma) {
		String tmp = "";
		if (!pinyinma.isEmpty()) {
			String upper_string = pinyinma.toUpperCase();
			byte[] h2 = upper_string.getBytes();			
			for (int i = 0; i < h2.length; i++) {
				int iasc = h2[i];
				char charater = upper_string.charAt(i);
				if (iasc <= 67 && iasc >= 65) {        // 'A' 'B' 'C'
					tmp = tmp + "2";
				} else if (iasc <= 70 && iasc >= 68) { // 'D' 'E' 'F'
					tmp = tmp + "3";
				} else if (iasc <= 73 && iasc >= 71) { // 'G' 'H' 'I'
					tmp = tmp + "4";
				} else if (iasc <= 76 && iasc >= 74) { // 'J' 'K' 'L'
					tmp = tmp + "5";
				} else if (iasc <= 79 && iasc >= 77) { // 'M' 'N' 'O' 
					tmp = tmp + "6";
				} else if (iasc <= 83 && iasc >= 80) { // 'P' 'Q' 'R' 'S'
					tmp = tmp + "7";
				} else if (iasc <= 86 && iasc >= 84) { // 'T' 'U' 'V'
					tmp = tmp + "8";
				} else if (iasc <= 90 && iasc >= 87) { // 'W' 'X' 'Y' 'Z'
					tmp = tmp + "9";
				} else {
					tmp = tmp + charater;
				}
			}
		}
		return tmp;
	}
}
