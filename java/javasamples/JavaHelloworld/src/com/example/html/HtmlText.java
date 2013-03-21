package com.example.html;

import java.util.regex.Pattern;

public class HtmlText {
	// 定义script的正则表达式{或<script[^>]*?>[\s\S]*?<\/script> }
	static String regEx_script = "<[\\s]*?script[^>]*?>[\\s\\S]*?<[\\s]*?\\/[\\s]*?script[\\s]*?>";
	// 定义style的正则表达式{或<style[^>]*?>[\s\S]*?<\/style> }
	static String regEx_style = "<[\\s]*?style[^>]*?>[\\s\\S]*?<[\\s]*?\\/[\\s]*?style[\\s]*?>";
	// 定义HTML标签的正则表达式
	static String regEx_html = "<[^>]+>";
	static String regEx_html1 = "<[^>]+";
	static java.util.regex.Pattern p_script;
	static java.util.regex.Matcher m_script;
	static java.util.regex.Pattern p_style;
	static java.util.regex.Matcher m_style;
	static java.util.regex.Pattern p_html;
	static java.util.regex.Matcher m_html;
	static java.util.regex.Pattern p_html1;
	static java.util.regex.Matcher m_html1;
	static {
		Pattern.compile(regEx_script, Pattern.CASE_INSENSITIVE);
		p_script = Pattern.compile(regEx_script, Pattern.CASE_INSENSITIVE);
		p_style = Pattern.compile(regEx_style, Pattern.CASE_INSENSITIVE);
		p_html = Pattern.compile(regEx_html, Pattern.CASE_INSENSITIVE);
		p_html1 = Pattern.compile(regEx_html1, Pattern.CASE_INSENSITIVE);
	}

	public static String Html2Text(String inputString) {
		String htmlStr = inputString.replaceAll("&lt;", "<").replaceAll("&nbsp;", " ");
		String textStr = "";

		try {
			m_script = p_script.matcher(htmlStr);
			htmlStr = m_script.replaceAll(""); // 过滤script标签
			m_style = p_style.matcher(htmlStr);
			htmlStr = m_style.replaceAll(""); // 过滤style标签
			m_html = p_html.matcher(htmlStr);
			htmlStr = m_html.replaceAll(""); // 过滤html标签
			m_html1 = p_html1.matcher(htmlStr);
			htmlStr = m_html1.replaceAll(""); // 过滤html标签
			textStr = htmlStr;
		} catch (Exception e) {
			System.err.println("Html2Text: " + e.getMessage());
		}
		return textStr;// 返回文本字符串
	}
	
	public static void main(String[] argc) {
		String text = "haha <a helloworld />";
		String pureText = HtmlText.Html2Text(text);
		System.out.println(pureText);
		
		System.out.println("\n");
		
		String t2 = "呵呵，全家幸福哦！||<a href=\"/null?reqcode=homepage&userid=fed822b32f3448efef7259760a5a0172\">@幽然一抹®徽</a>&nbsp;一大家人窝在爷爷房间吃吃喝喝聊聊，看春晚，四世同堂，爷爷听力不好'不怎么说话，但是脸上一直洋溢着幸福，满足^龙年了'祝愿我们全家年年如此'爷爷身体健康@";
		String pt2 = HtmlText.Html2Text(t2);
		System.out.println(pt2);
	}
}