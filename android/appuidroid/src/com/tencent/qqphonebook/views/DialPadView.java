package com.tencent.qqphonebook.views;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.widget.LinearLayout;

public class DialPadView extends LinearLayout {
	public DialPadView(Context context, AttributeSet attrSet) {
		super(context, attrSet);
	}
	
	public boolean dispatchTouchEvent(MotionEvent motionEvent) {
		return super.dispatchTouchEvent(motionEvent);
	}
	
	public boolean onInterceptTouchEvent(MotionEvent motionEvnet){
		return false;
	}
	
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		super.onLayout(changed, l, t, r, b);
	}
}