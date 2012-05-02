package com.example.baikudroid.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.widget.AbsListView.OnScrollListener;
import android.widget.AbsListView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

public class PullToRefreshListView extends ListView implements 
	OnScrollListener, GestureDetector.OnGestureListener {
	
	private OnRefreshListener mOnRefreshListener;
	private OnScrollListener mOnScrollListener;
	private LayoutInflater mInflater;
	private GestureDetector mDetector;
	
	// View
	private LinearLayout mRefreshView;
	private TextView mRefreshViewText;
	private ImageView mRefreshViewImage;
	private ProgressBar mRefreshViewProgress;
	private TextView mRefreshViewLastUpdated;
	
	public interface OnRefreshListener {
		public void onRefresh();
	}
	
	public PullToRefreshListView(Context context) {
		super(context);
	}
	
	public PullToRefreshListView(Context context, AttributeSet attrs){
		super(context, attrs);
	}
	
	public PullToRefreshListView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}
	
	public void setOnRefreshListener(OnRefreshListener onRefreshListenr) {
		mOnRefreshListener = onRefreshListenr;
	}
	
	@Override
	public void setAdapter(ListAdapter adapter) {
		super.setAdapter(adapter);
		setSelection(1);
	}
	
	/**
	 * Set the listener that will receive notifications every time the list scrolls.
	 */
	@Override
	public void setOnScrollListener(AbsListView.OnScrollListener l) {
		
	}
	
	@Override
	public void onScroll(AbsListView view, int firstVisibleItem,
			int visibleItemCount, int totalItemCount) {
	}
	
	@Override
	public void onScrollStateChanged(AbsListView view, int scrollState) {
		
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		return super.onTouchEvent(event);
	}
	
	@Override
	public boolean onDown(MotionEvent e) {
		return false;
	}
	
	@Override
	public void onShowPress(MotionEvent e) {
		
	}
	
	@Override
	public void onLongPress(MotionEvent e) {
		
	}
	
	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, 
			float distanceX, float distanceY) {
		return false;
	}
	
	@Override
	public boolean onSingleTapUp(MotionEvent e) {
		return false;
	}
	
	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, 
			float velocityX, float velocityY) {
		return false;
	}
};