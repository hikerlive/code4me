package com.example.algorithm;

/**
 * 折半搜索、也称二分查找算法、二分搜索，是一种在有序数组中查找某一特定元素的搜索算法。
 * 算法思想：
 *   1.搜索过程从数组的中间元素开始，如果中间元素正好是要查找的元素，则搜索过程结束。
 *   2.如果某一特定元素大于或者小于中间元素，则在数组大于或小于中间元素的那一半中查找，
 *     而且跟开始一样从中间元素开始比较。如果在某一步骤数组为空，则代表找不到。
 * 优势:
 * 	这种搜索算法每一次比较都使搜索范围缩小一半。
 * 
 * 时间：
 * 	2013-03-21
 */
public class HalfSeek {
	
	private static int[] abc = new int[] {9, 8, 7, 6, 5, 4, 3, 2, 1};
	public static void main(String[] argc) {
		int ret = half_seek(abc, 0, 8, 3);
		System.out.print("half_seek ret:" + ret);
	}
	
	public static int half_seek(int arr[], int low, int high, int num) {
		if ((low >= high) && (arr[low] != num))
			return -1;
		
		int mid;
		mid = (low + high) / 2;
		
		if (arr[mid] == num) {
			return mid;
		}
		else if (arr[mid] > num) {
			high = mid - 1;
		} else {
			low = mid + 1;
		}
		return half_seek(arr, low, high, num);
	}
}