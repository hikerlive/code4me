package com.example.appuidroid.graphics;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.MotionEvent;
import android.view.View;

public class DrawView extends View implements View.OnTouchListener{
	
	List<Point> points = new ArrayList<Point>();
	Paint paint = new Paint();
	
	public DrawView(Context context) {
		super(context);
		setFocusable(true);
		setFocusableInTouchMode(true);
		
		this.setOnTouchListener(this);
		
		paint.setColor(Color.WHITE);
		paint.setAntiAlias(true);
	}
	
	@Override
	public void onDraw(Canvas canvas) {
		for (Point point : points) {
			canvas.drawCircle(point.x, point.y, 5, paint);
		}
	}
	
	public boolean onTouch(View view, MotionEvent event) {
		Point point = new Point();
		point.x = event.getX();
		point.y = event.getY();
		points.add(point);
		invalidate();
		return true;
	}
	
	private class Point{
		float x, y;
		
		@Override
		public String toString() {
			return x + ", " + y;
		}
	}
}
