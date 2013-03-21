package com.example.test;

import java.util.ArrayList;
import java.util.List;

/**
 * java对象只要是new出来在堆中，其它地方均是对其的引用，一处修改其它引用均可见。 
 */
public class ArrayListReference {
	public List<String> msgids;
	public List<Abc> abcs;
	public ArrayListReference() {
		msgids = new ArrayList<String>();
		msgids.add("one1");
		msgids.add("one2");
		
		abcs = new ArrayList<Abc>();
		abcs.add(new Abc(1, 10));
		abcs.add(new Abc(2, 4));
	}
	
	public void testcase() {
		String temp = msgids.get(0);
		temp = "100";
		System.out.print(msgids.get(0));
		System.out.print("\n");
		
		abcs.get(0).print();
		
		System.out.print("\n");
		Abc abc = abcs.get(0);
		abc.a = 5;
		abc.b = 6;
		abcs.get(0).print();
	}
	
	public class Abc {
		int a;
		int b;
		public Abc(int i, int j) {
			a = i;
			b = j;
		}
		
		public void print() {
			System.out.print("a=" + a + ",b=" + b);
		}
	};
	
	public static void main(String[] args) {
		ArrayListReference lr = new ArrayListReference();
		lr.testcase();
	}
}