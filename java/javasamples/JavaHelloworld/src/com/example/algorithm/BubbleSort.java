package com.example.algorithm;

/**
 * 该示例是冒泡排序算法的示例
 * 算法思想：
 *   1.轻气泡不能在重气泡之下
 *   2.第二轮找出最小的气泡放在第一轮开始的位置元素中。
 *   
 * 每一趟有序区增加一个气泡、无序区减少一个气泡。
 * 
 * 时间复杂度：最差时间复杂度：O(n2)\最优时间复杂度：O(n)
 * 最差空间复杂度：O(n)、需要辅助空间O(1)
 * 并不是最佳算法：no
 */
public class BubbleSort {
	public static void main(String[] argc) {
		int[] abc = new int[10];
		for (int i=9; i>=0; i--) {
			abc[i] = i;
			System.out.print(abc[i] + ",");
		}
		System.out.print("\n");
		
		for (int i=0; i<abc.length; i++) {
			int j=i;
			for (j++; j<abc.length; j++) {
				if (abc[i] > abc[j]) {
					int tmp = abc[i];
					abc[i] = abc[j];
					abc[j] = tmp;
				}
			}
		}
		
		for (int i=0; i<abc.length; i++) {
			System.out.print(abc[i] + ",");
		}
		System.out.print("\n");
	}
}