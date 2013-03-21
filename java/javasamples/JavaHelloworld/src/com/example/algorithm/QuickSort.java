package com.example.algorithm;

/**
 * 交换排序中快速排序方法，即二分法查找
 * 算法思想： 
 *    1.找出基准位置
 *    2.分成两个更小的区间，左区间的元素要全部小于基准位置数据，右区间的元素要全部大于基准位置数据。
 *    3.左区间与右区间各再进行一次递归调用。
 *   
 */
public class QuickSort {
	private static int[] abc = new int[] { 2, 1, 5, 4, 9, 8, 6, 7, 10, 3 }; 
	public static void main(String[] argc) {
		print();
		quickSort(abc, 0, abc.length - 1);
		print();
	}
	
	public static void quickSort(int abc[], int from, int to) {
		if (from < to) {
			int temp = abc[to];
			int i = from -1;
			for (int j=from; j<to; j++) {
				if (abc[j] <= temp) {
					i++;
					int tmp = abc[j];
					abc[j] = abc[i];
					abc[i] = tmp;
				}
			}
			abc[to] = abc[i+1];
			abc[i+1] = temp;
			
			quickSort(abc, from, i);
			quickSort(abc, i+1, to);
		}
	}
	
	public static void print() {
		for (int i : abc) {
			System.out.print(i + ",");
		}
		System.out.print("\n");
	}
}