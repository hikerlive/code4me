package com.example.appuidroid;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.VelocityTracker;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Scroller;

public class WeChatScrollLayout extends ViewGroup{
	private static final String TAG = "WeChatScrollLayout";
	private VelocityTracker mVelocityTracker; //用于判断甩动手势 
	private Scroller mScroller; // 滑动控制
	private int mCurScreen;
	private float mLastMotionX;
	
	private WeChatViewChangeListener mOnViewChangeListener;
	public void SetOnViewChangeListener(WeChatViewChangeListener listener) {
		mOnViewChangeListener = listener;
	}
	 
	public WeChatScrollLayout(Context context) {
		super(context);
		mScroller = new Scroller(context);
		mCurScreen = 0;
	}
	public WeChatScrollLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
		mScroller = new Scroller(context);
		mCurScreen = 0;
	}
	public WeChatScrollLayout(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		mScroller = new Scroller(context);
		mCurScreen = 0;
	}

	/**
	 * Called from layout when this view should assign a size and position 
	 * to each of its children.
	 */
	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		if (changed) {
			int childLeft = 0;
			final int childCount = getChildCount();
			for (int i=0; i<childCount; i++) {
				final View childView = getChildAt(i);
				if (childView.getVisibility() != View.GONE) {
					final int childWidth = childView.getMeasuredWidth();
					childView.layout(childLeft, 0, childLeft + childWidth, 
							childView.getMeasuredHeight());
					childLeft += childWidth;
				}
			}
		}
	}
	
	/**
	 * Measure the view and its content to determine the measured width
	 * and the measured height.
	 */
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		
		final int width = MeasureSpec.getSize(widthMeasureSpec);
		final int widthMode = MeasureSpec.getMode(widthMeasureSpec);
		
		final int count = getChildCount();
		for (int i=0; i<count; i++) {
			getChildAt(i).measure(widthMeasureSpec, heightMeasureSpec);
		}
		scrollTo(mCurScreen * width, 0);
	}
	
	/**
	 * Called by a parent to request that a child update its values for
	 * mScrollX and mScrollY if necessary.
	 */
	@Override
	public void computeScroll() {
		if (mScroller.computeScrollOffset()) {
			scrollTo(mScroller.getCurrX(), mScroller.getCurrY());
			postInvalidate();
		}
	}
	
	/**
	 * Implement this method to handle touch screen motion event.
	 */
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		final int action = event.getAction();
		final float x = event.getX();
		final float y = event.getY();
		
		switch (action) {
		case MotionEvent.ACTION_DOWN:
			Log.d(TAG, "ACTION_DOWN");
			if (mVelocityTracker == null) {
				mVelocityTracker = VelocityTracker.obtain();
				mVelocityTracker.addMovement(event);
			}
			if (!mScroller.isFinished()) {
				mScroller.abortAnimation();
			}
			mLastMotionX = x;
			break;
		case MotionEvent.ACTION_MOVE:
			int deltaX = (int) (mLastMotionX -x);
			if (isCanMove(deltaX)) {
				if (mVelocityTracker != null) {
					mVelocityTracker.addMovement(event);
				}
				mLastMotionX = x;
				scrollBy(deltaX, 0);
			}
			break;
		case MotionEvent.ACTION_UP:
			int velocityX = 0;
			if (mVelocityTracker != null) {
				mVelocityTracker.addMovement(event);
				mVelocityTracker.computeCurrentVelocity(1000);
				velocityX = (int) mVelocityTracker.getXVelocity();
			}
			if (velocityX > 600 && mCurScreen > 0) {
                Log.e(TAG, "snap left");    
                snapToScreen(mCurScreen - 1);
			} else if (velocityX < -600 && mCurScreen < getChildCount() -1) {
				Log.e(TAG, "snap right");
				snapToScreen(mCurScreen + 1);
			} else {
				snapToDestination();
			}
			if (mVelocityTracker != null) {
				mVelocityTracker.recycle();
				mVelocityTracker = null;
			}
			break;
		}
		return true;
	}
	
	private boolean isCanMove(int deltaX) {
		if (getScrollX() <=0 && deltaX < 0) {
			return false;
		}
		if (getScrollX() >= ((getChildCount() -1) * getWidth()) 
				&& deltaX > 0){
			return false;
		}
		return true;
	}
	
	 public void snapToDestination() {    
	        final int screenWidth = getWidth();    

	        final int destScreen = (getScrollX()+ screenWidth/2)/screenWidth;    
	        snapToScreen(destScreen);    
	 }  
	
	 public void snapToScreen(int whichScreen) {    
		// get the valid layout page    
		whichScreen = Math.max(0, Math.min(whichScreen, getChildCount()-1));    
		if (getScrollX() != (whichScreen*getWidth())) {    
		    final int delta = whichScreen*getWidth()-getScrollX();    
		    mScroller.startScroll(getScrollX(), 0,
		    		delta, 0, Math.abs(delta)*2);
		    mCurScreen = whichScreen;    
		    invalidate();       // Redraw the layout    
		    if (mOnViewChangeListener != null) {
		    	mOnViewChangeListener.OnViewChange(mCurScreen);
		    }
		}
	 }  
	
	
}
