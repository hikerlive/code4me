package com.example.algorithm;

import java.util.ArrayList;
import java.util.List;

import net.sourceforge.pinyin4j.PinyinHelper;
import net.sourceforge.pinyin4j.format.HanyuPinyinCaseType;
import net.sourceforge.pinyin4j.format.HanyuPinyinOutputFormat;
import net.sourceforge.pinyin4j.format.HanyuPinyinToneType;
import net.sourceforge.pinyin4j.format.exception.BadHanyuPinyinOutputFormatCombination;

public class T9Word {
	private boolean isChineseWord; // 是否是汉字
	private int prevSpecCount; // 前有几个特殊字符
	private boolean isAllPrevSpace; // 前有几个特殊字符是否全是空格
	private T9Word nextLink= null;
	private T9Word prevLink= null;
	private String self; // 词本向
	
	private List<String> shortpys;
	private List<String> quanpys;
	private List<T9PinYin> pinyinitems;
	private List<T9PinYin> quanpyitems;
	private List<T9PinYin> pinyins;
	
	private static HanyuPinyinOutputFormat hanyuOutput = new HanyuPinyinOutputFormat();

	public T9Word(String word, boolean isChinese) {
		this.isChineseWord = isChinese;
		this.self = (isChinese) ? word : word.toUpperCase();
		hanyuOutput.setCaseType(HanyuPinyinCaseType.LOWERCASE);
		hanyuOutput.setToneType(HanyuPinyinToneType.WITHOUT_TONE);
	}
	
	public void setPrevSpecCount(int specCount) {
		this.prevSpecCount = specCount;
	}
	
	public void setAllPrevSpace(boolean prevSpace) {
		this.isAllPrevSpace = prevSpace;
	}
	
	public boolean isChineseWord() {
		return isChineseWord;
	}
	
	public void convertWord() {
		if (isChineseWord) {
			char c = self.charAt(0);
			try {
				String[] pys = PinyinHelper.toHanyuPinyinStringArray(c, hanyuOutput);
				if (null != pys) {
					shortpys = new ArrayList<String>();
					quanpys = new ArrayList<String>();
					
					for (int i=0; i<pys.length; i++) {
						String py = pys[i].toUpperCase();
						quanpys.add(py);
						
						String first = py.substring(0, 1);
						if (!shortpys.contains(first)) {
							shortpys.add(first);
						}
						if (py.length() > 2) {
							if ('H' == py.charAt(1)) {
								String second = py.substring(0, 2);
								if (!shortpys.contains(second)) {
									shortpys.add(second);
								}
							}
						}
					}
				}
			} catch (BadHanyuPinyinOutputFormatCombination e) {
				e.printStackTrace();
			}
		} 
	}
	
	public boolean isChineseLink() {
		if (isChineseWord) {
			if (prevSpecCount == 0) {
				return true;
			} 
			else if (prevSpecCount > 0) {
				return isAllPrevSpace;
			}
		}
		return false;
	}
	
	public void setNextLink(T9Word word) {
		nextLink = word;
	}
	
	public T9Word getNextLink() {
		return nextLink;
	}
	
	public void setPrevLink(T9Word word) {
		prevLink = word;
	}
	
	public T9Word getPrevLink() {
		return prevLink;
	}

	public List<T9PinYin> getPinYinItems() {
		if (pinyinitems != null) {
			return pinyinitems;
		}
		
		pinyinitems = new ArrayList<T9PinYin>();
		if (isChineseWord) {
			for (int i=0; i<shortpys.size(); i++) {
				String str = shortpys.get(i);
				pinyinitems.add(new T9PinYin(getKey(str),true));
			}
			for (int i=0; i<quanpys.size(); i++) {
				String str = quanpys.get(i);
				pinyinitems.add(new T9PinYin(getKey(str),false));
			}
		}
		
		return pinyinitems;
	}
	
	public List<T9PinYin> getPinYinItems2() {
		if (pinyins != null) {
			return pinyins;
		}
		
		pinyins = new ArrayList<T9PinYin>();
		if (isChineseWord) {
			for (int i=0; i<shortpys.size(); i++) {
				String str = shortpys.get(i);
				pinyins.add(new T9PinYin(str,true));
			}
			for (int i=0; i<quanpys.size(); i++) {
				String str = quanpys.get(i);
				pinyins.add(new T9PinYin(str,false));
			}
		}
		
		return pinyins;		
	}
	
	public List<T9PinYin> getQuanPinYinItems() {
		if (quanpyitems != null) {
			return quanpyitems;
		}
		
		quanpyitems = new ArrayList<T9PinYin>();
		if (isChineseWord) {
			for (int i=0; i<quanpys.size(); i++) {
				String str = quanpys.get(i);
				quanpyitems.add(new T9PinYin(getKey(str),false));
			}
		} else {
			quanpyitems.add(new T9PinYin(getKey(self),false));
		}
		
		return quanpyitems;
	}
	
	public String getKey(String str) {
		String key="";
		if (prevSpecCount > 0) {
			for (int i=0; i<prevSpecCount; i++) {
				key += " ";
			}
		}
		key += str;
		return key;
	}
	
	public String toString() {
		return "word=["
			+ "self=" + self
			+ ",isChineseWord=" + isChineseWord
			+ ",prevSpecCount=" + prevSpecCount
			+ ",isAllPrevSpace=" + isAllPrevSpace
			+ "]\n";
	}
}