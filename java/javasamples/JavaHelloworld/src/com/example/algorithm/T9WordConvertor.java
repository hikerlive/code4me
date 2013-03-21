package com.example.algorithm;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

public class T9WordConvertor {
	private String name;
	private List<T9Word> t9words;
	public String sortkey3="";
	public String sortpinyin="";
	private static final Pattern pattern = Pattern.compile("^[A-Za-z0-9]+$");
	
	public T9WordConvertor(String name) {
		this.name = name;
		t9words = new ArrayList<T9Word>();
		divideWord();
		parseWord();
	}
	
	/**
	 * 将文字按汉字、拼音数字、特殊字符分成多个词并记录每个词的状态
	 * @return
	 */
	public void divideWord() {
		if (name.isEmpty()) {
			return;
		}
		
		StringBuffer pybf = new StringBuffer();
		char[] arr = name.toCharArray();
		int pretype = -1;
		String word = "";
		String specword = "";
		for (int i=0; i<arr.length; i++) {
			char c = arr[i];
			if (c > 128) { // 汉字
				if (pretype == -1) { pretype = 1; }
				T9Word hanzi = new T9Word(Character.toString(c), true);
				if (pretype == 2) {
					T9Word pinyin = new T9Word(word, false);
					pinyin.setPrevSpecCount(specword.length());
					pinyin.setAllPrevSpace(isAllSpace(specword));
					specword = "";
					t9words.add(pinyin);
					word = "";
				}
				else if (pretype == 3) {
					hanzi.setPrevSpecCount(word.length());
					hanzi.setAllPrevSpace(isAllSpace(word));
					word = "";
				}
				t9words.add(hanzi);
				pretype = 1;
			} 
			else if (pattern.matcher(Character.toString(c)).matches()) { // 拼音与数字
				if (pretype == -1) { pretype = 1; }
				if (pretype == 3) {
					specword = word;
					word = "";
				}
				word += c;
				pretype = 2;
			}
			else { // 特殊字符不做处理
				if (pretype == 2) {
					T9Word pinyin = new T9Word(word, false);
					pinyin.setPrevSpecCount(specword.length());
					pinyin.setAllPrevSpace(isAllSpace(specword));
					specword = "";
					t9words.add(pinyin);
					word = "";
				}
				word += c;
				pretype = 3;
			}
			if (i == arr.length - 1) {
				if (pretype == 2) {
					T9Word pinyin = new T9Word(word, false);
					pinyin.setPrevSpecCount(specword.length());
					pinyin.setAllPrevSpace(isAllSpace(specword));
					specword = "";
					t9words.add(pinyin);
					word = "";
				}
			}
		}
		pybf = null;
	}
	
	/**
	 * 将词的连续性补充上
	 * 如游;振雄
	 *   振雄这两个字可被缩写检索到
	 */
	public void parseWord() {
		int count = t9words.size();
		for (int i=0; i<count; i++) {
			T9Word word = t9words.get(i);
			word.convertWord();
			
			if (i+1 < count) {
				T9Word next = t9words.get(i+1);
				if (next.isChineseLink()) {
					word.setNextLink(next);
					next.setPrevLink(word);
				}
			}
		}
	}
	
	/**
	 * 生成sortkey3，规则如下
	 * 1.游振雄
	 *   Y Z X\tY ZH X\tYOU Z X\tYOU ZH X\tYOU ZHEN X\tYOU ZHEN XIONG\t
	 *   
	 * 2.游;振雄
	 *    Z X\t ZH X\t ZHEN X\t ZHEN XIONG\tYOU  ZHEN XIONG\t
	 *    
	 * 3.youzx@channelsoft.com
	 *   YOUZX CHANNELSOFT COM\t
	 *   
	 * 4.youzx@163.com
	 *   YOUZX 163 COM\t
	 *   
	 * 5.游HAO123雄
	 *   YOU HAO123 XIONG\t
	 *   
	 * @return
	 */
	public String getSortKey3() {
		if (null == t9words || t9words.isEmpty()) {
			return "";
		}
		
		// step1:找出可缩写链表
		List<T9Word> cwords = new ArrayList<T9Word>();
		for (T9Word word: t9words) {
			if (null == word.getPrevLink() && null != word.getNextLink()) {
				cwords.add(word);
			}
		}
		
		// step2:可缩写链表拼音
		List<T9PinYin> allitems = new ArrayList<T9PinYin>();
		for(T9Word word : cwords) {
			List<T9PinYin> items = word.getPinYinItems();
			T9Word next = word.getNextLink();
			while (null != next) {
				items = mergePinYin(items, next.getPinYinItems());
				next = next.getNextLink();
			}
			allitems.addAll(items);
		}
		
		// step3:所有词的全拼
		boolean isfulllink = true;
		if (cwords.size() == 1) {
			T9Word word = cwords.get(0);
			T9Word next = word.getNextLink();
			int count = 1;
			while (null != next) {
				count ++;
				next = next.getNextLink();
			}
			if (count == t9words.size()) {
				isfulllink = false;
			}
		}
		if (isfulllink) {
			List<T9PinYin> items = t9words.get(0).getQuanPinYinItems();
			for (int i=0; i<t9words.size()-1; i++) {
				T9Word next = t9words.get(i+1);
				items = mergePinYin(items, next.getQuanPinYinItems());
			}
			allitems.addAll(items);
		}
		
		for (T9PinYin item : allitems) {
			sortkey3 += item.str + "\t";
		}
		
		cwords.clear();
		cwords = null;
		allitems.clear();
		allitems = null;
	
		return sortkey3;
	}
	
	/**
	 * 生成sortkey，规则如下：
	 * 1.游振雄
	 *   YZX YOUZX YOUZHX YOUZHENX YOUZHENXIONG
	 * @return
	 */
	public String getSortPinYin() {
		if (t9words.isEmpty()) {
			return "";
		}
		List<T9PinYin> items = null;
		for (int i=0; i<t9words.size()-1; i++) {
			T9Word t9word = t9words.get(i);
			if (!t9word.isChineseWord()) {
				continue;
			}
			if (items == null) {
				items = t9word.getPinYinItems2();
			}
			T9Word next = t9words.get(i+1);
			if (!next.isChineseWord()) {
				continue;
			}
			items = mergePinYin2(items, next.getPinYinItems2());
		}
		for (T9PinYin item : items) {
			sortpinyin += item.str + " ";
		}
		items.clear();
		items = null;
		
		return sortpinyin;
	}
	
	/**
	 * 打印分词结果
	 */
	private void printWord() {
		for (T9Word word : t9words) {
			System.out.print(word.toString());
		}
	}
	
	/**
	 * 合并两个词的拼音部分
	 * @param ids1
	 * @param ids2
	 * @return
	 */
	private List<T9PinYin> mergePinYin(List<T9PinYin> ids1, List<T9PinYin> ids2) {
		List<T9PinYin> nids = new ArrayList<T9PinYin>();
		for (T9PinYin id1 : ids1) {
			for (T9PinYin id2 : ids2) {
				if (id1.shuoxie) {
					if (id2.shuoxie) {
						nids.add(new T9PinYin((id1.str +" "+ id2.str), true));
					}
				} else {
					boolean shuoxie = id1.shuoxie || id2.shuoxie;
					nids.add(new T9PinYin((id1.str + " " + id2.str), shuoxie));
				}
			}
		}
		return nids;
	}
	
	/**
	 * 合并两个词的拼音部分
	 * @param ids1
	 * @param ids2
	 * @return
	 */
	private List<T9PinYin> mergePinYin2(List<T9PinYin> ids1, List<T9PinYin> ids2) {
		List<T9PinYin> nids = new ArrayList<T9PinYin>();
		for (T9PinYin id1 : ids1) {
			for (T9PinYin id2 : ids2) {
				if (id1.shuoxie) {
					if (id2.shuoxie) {
						nids.add(new T9PinYin((id1.str + id2.str), true));
					}
				} else {
					boolean shuoxie = id1.shuoxie || id2.shuoxie;
					nids.add(new T9PinYin((id1.str + id2.str), shuoxie));
				}
			}
		}
		return nids;
	}
	
	public static boolean isAllSpace(String word) {
		if (word.isEmpty()) {
			return false;
		}
		
		String str = word.trim();
		return str.isEmpty();
	}
	
	public static String pinyin2Keycodes(String pinyin) {
		String tmp = "";
		byte[] h2 = pinyin.getBytes();			
		for (int i = 0; i < h2.length; i++) {
			int iasc = h2[i];
			char charater = pinyin.charAt(i);
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
		return tmp;
	}

	/**
	 * sortkey3
	 *   游 振雄
	 *   	y  z x\tyou  z x\tyou  zh x\tyou  zhen x \tyou  zhen xiong\t
	 *   游hao123振雄
	 *      YOU HAO123 ZHEN XIONG\t
	 *   刘@123德华
	 *   	D H\tDE H\tDE HUA\tLIU  123 DE HUA\t
	 *   gonggai@163.com
	 *   	GONGGAI  163  COM\t
	 * @param argc
	 */
	public static void main(String[] argc) {
		T9WordConvertor creator = new T9WordConvertor("游hao123振雄");
		creator.printWord();
		String sortkey3 = creator.getSortKey3();
		System.out.print(sortkey3);
		
		/*
		String sortkey3codes = pinyin2Keycodes(sortkey3);
		System.out.print("\n");
		System.out.print(sortkey3codes);
		String sortpinyin = creator.getSortPinYin();
		System.out.print("\n");
		System.out.print(sortpinyin);
		*/
	}
}