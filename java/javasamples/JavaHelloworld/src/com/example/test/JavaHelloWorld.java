package com.example.test;
import java.util.ArrayList;
import java.util.List;

/**
 * 该示例解决一个什么样的问题呢？
 * 	1.验证java中是不有类似于C++的函数参数引用传值的功能？答案是没有。
 *  	解决方法：可以通过对象来包装，以对象的形式将该值传入。
 */
public class JavaHelloWorld {
	private static JavaHelloWorld instance = null;
	public static JavaHelloWorld getInstance() {
		if (instance == null) {
			instance = new JavaHelloWorld();
		}
		return instance;
	}
	
	public static void main(String[] args) {
		List<String> abc = null;
		getInstance().function(abc);
		if (null != abc) {
			for (String str : abc) {
				System.out.print(str);
			}
		} else {
			System.out.print("get function param list [abc] is null\n");
		}
		
		JavaObject object = null;
		getInstance().function1(object);
		if (null != object) {
			System.out.print("object is valid\n");
		} else {
			System.out.print("object is null\n");
		}
		
		JavaObject object2 = getInstance().new JavaObject();
		getInstance().function2(object2);
		if (null != object2) {
			for (String str : object2.objList) {
				System.out.print(str);
			}
		} else {
			System.out.print("object2 is null\n");
		}
	}
	
	/**
	 * JavaObject类声明放在这里，在static函数中new JavaObject()会报如下错误：
	 * No enclosing instance of type JavaHelloWorld is accessible. 
	 * Must qualify the allocation with an enclosing instance of type JavaHelloWorld (e.g. x.new A() where x is an instance of JavaHelloWorld)
	 */
	public class JavaObject {
		public JavaObject() {
		}
		public List<String> objList;
	} 
	public void function(List<String> abc) {
		abc = new ArrayList<String>();
		abc.add("helloworld");
	}
	public void function1(JavaObject obj) {
		obj = new JavaObject();
	}
	public void function2(JavaObject obj) {
		obj.objList = new ArrayList<String>();
		obj.objList.add("helloworld");
	}
}