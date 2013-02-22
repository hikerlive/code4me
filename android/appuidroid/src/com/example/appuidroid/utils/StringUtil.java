package com.example.appuidroid.utils;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;

import android.text.TextUtils;

public class StringUtil {
	public static String toString(List<String> array) {
		if (null == array) {
			return "";
		}
		
		int i=0;
		String str = "";
		for (String one : array) {
			if (i != 0) {
				str += ",";
			}
			str += "\'" + one + "\'";
			i++;
		}
		
		return str;
	}
	
	public static List<String> toArray(String str) {
		if (TextUtils.isEmpty(str)) {
			return null;
		}
		
		List<String> strs = new ArrayList<String>();
		int pos1=0, pos2=0;
		while ((pos2 = str.indexOf(",", pos1)) != -1) {
			String single = str.substring(pos1+1, pos2-1);
			strs.add(single);
			pos1 = pos2+1;
		}
		
		if (pos2 == -1) {
			if (pos1 == 0) {
				str = str.substring(1, str.length()-1);
				strs.add(str);
			} else {
				String single = str.substring(pos1);
				single = single.substring(1, single.length()-1);
				strs.add(single);
			}
		}
		return strs;
	}
	
	public static JSONArray toJSONArray(List<String> strs) {
		JSONArray array = new JSONArray();
		for (String str : strs) {
			array.put(str);
		}
		return array;
	}
	
	/**
	 * sort array by little to big.
	 * @param strs
	 * @return
	 */
	public static List<String> sortArray(List<String> strs) {
		for (int i=0; i<strs.size(); i++) {
			for (int j=i; j<strs.size(); j++) {
				if (strs.get(i).compareTo(strs.get(j)) > 0) {
					String temp = strs.get(i);
					strs.set(i, strs.get(j));
					strs.set(j, temp);
				}
			}
		}
		return strs;
	}
}