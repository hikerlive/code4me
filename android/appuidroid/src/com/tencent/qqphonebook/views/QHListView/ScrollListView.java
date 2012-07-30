package com.tencent.qqphonebook.views.QHListView;

import android.content.Context;
import android.util.AttributeSet;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.widget.ListView;

public class ScrollListView extends ListView implements GestureDetector.OnGestureListener {
	public ScrollListView(Context context, AttributeSet attrSet) {
		super(context, attrSet);
	}
	
	public boolean onDown(MotionEvent e) {
		return false;
	}
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
		return true;
	}
	public void onLongPress(MotionEvent e) {
		
	}
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
		return false;
	}
	public void onShowPress(MotionEvent e){
		
	}
	public boolean onSingleTapUp(MotionEvent e){
		return false;
	}
	
}