package com.example.appuidroid;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class LetterListView extends View{
	private int choose = -1;
	private boolean isShowBkg = true;
	private Paint paint = new Paint();
	private LetterListViewListener letterChangedListener;
	
	private String[] letters = {"佰", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
			"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W",
			"X", "Y", "Z", "#"};
	
	
	public LetterListView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}
	
	public LetterListView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	public LetterListView(Context context) {
		super(context);
	}
	
	/**
	 * Called when the view should render its content. 
	 */
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		if (letters.length == 0) {
			return;
		}
		
		if (isShowBkg) {
			canvas.drawColor(Color.parseColor("#40000000"));
		}
		
		int height = getHeight() - 20;
		int width = getWidth();
		int itemHeight = height / letters.length;
		for (int i=0; i<letters.length; i++) {
			paint.setColor(Color.parseColor("#686D73"));
			paint.setTypeface(Typeface.DEFAULT_BOLD);
			paint.setAntiAlias(true);
			if (i == choose) {
				paint.setColor(Color.parseColor("#40000000"));
				paint.setFakeBoldText(true);
			}
			float xPos = width / 2 - paint.measureText(letters[i]);
			float yPos = itemHeight * i + itemHeight /2 + 20;
			canvas.drawText(letters[i], xPos, yPos, paint);
			paint.reset();
		}
	}
	
	@Override
	public boolean dispatchTouchEvent(MotionEvent event) {
		final int action = event.getAction();
		final float y = event.getY(); 
		final int itemHeight = getHeight() / letters.length;
		final int c = (int) ((y-itemHeight/2)/itemHeight + 0.5); // 点击的字母在list列表中位置 
		final int old = choose;
		
		switch (action) {
		/**
		 * A pressed gesture has started, the motion contains the initial
		 * starting location.
		 */
		case MotionEvent.ACTION_DOWN:
			if (old != c && letters != null) {
				if (c > 0 && c < letters.length) {
					letterChangedListener.onTouchingLetterChanged(letters[c]);
				   choose = c;
				   invalidate();
				}
			}
			break;
			
		/**
		 * A change has happened during a press gesture 
		 * (between ACTION_DOWN and ACTION_UP)
		 */
		case MotionEvent.ACTION_MOVE:
			if (old != c && letters != null) {
				if (c > 0 && c < letters.length) {
					letterChangedListener.onTouchingLetterChanged(letters[c]);
					choose = c;
					invalidate();
				}
			}
			break;
			
		/**
		 * A pressed gesture has finished, the motion contains
		 * the final release location as well as any intermediate points since the last down
		 * or move event.
		 */
		case MotionEvent.ACTION_UP:
			choose = -1;
			invalidate();
			break;
		}
		return true;
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		return super.onTouchEvent(event);
	}
	
	public void setListener(LetterListViewListener listener) {
		this.letterChangedListener = listener;
	}
}
