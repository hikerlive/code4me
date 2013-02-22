package com.example.appuidroid;

import android.R.color;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class CanvasDrawView extends SurfaceView implements SurfaceHolder.Callback{
	private CanvasThread canvasThread;
	
	public CanvasDrawView(Context context) {
		super(context);
	}
	
	public CanvasDrawView(Context context, AttributeSet attrs) {
		super(context, attrs);
		this.getHolder().addCallback(this);
		this.canvasThread = new CanvasThread(getHolder());
		this.setFocusable(true);
	}
	
	public CanvasDrawView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}
	
	public void startDrawImage() {
		canvasThread.setRunning(true);
		canvasThread.start();
	}
	
	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
		
	}
	
	public void surfaceCreated(SurfaceHolder arg0){
		
	}
	
	public void surfaceDestroyed(SurfaceHolder arg0) {
		boolean retry = true;
		canvasThread.setRunning(false);
		while (retry) {
			try {
				canvasThread.join();
				retry = false;
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		Bitmap sweet = BitmapFactory.decodeResource(getResources(), R.drawable.sq);
		canvas.drawColor(color.black);
		canvas.drawBitmap(sweet, 0, 0, null);
		
		Paint paint = new Paint();
		
		// draw a red circle
		paint.setColor(Color.RED);
		canvas.drawCircle(20, 50, 25, paint);
		
		// a blue circle with antialiasing turned on
		paint.setAntiAlias(true);
		paint.setColor(Color.BLUE);
		canvas.drawCircle(60, 50, 25, paint);
		
		// Now we wiil create some triangles.Create a Path object to store
		// out triangle's segments. Use.offset to draw in many locations.
		paint.setStyle(Paint.Style.FILL_AND_STROKE);
		paint.setStrokeWidth(2);
		paint.setColor(Color.GREEN);
		Path path = new Path();
		path.moveTo(4, -10);
		path.lineTo(20, 0);
		path.lineTo(-9, 0);
		path.close();
		path.offset(60, 40);
		canvas.drawPath(path, paint);
		path.offset(90, 100);
		canvas.drawPath(path, paint);
		path.offset(80, 150);
		canvas.drawPath(path, paint);
		
		// Now draw a text using FILL style:
		paint.setStyle(Paint.Style.FILL);
		paint.setAntiAlias(true);
		paint.setTextSize(20);
		canvas.drawText("Hello Android! Fill...", 50, 230, paint);
		
		// rotate the text, and you can draw it where you want:
		int x = 75;
		int y = 185;
		paint.setColor(Color.GRAY);
		paint.setTextSize(25);
		String rotatedtext = "Rotated hellowandroid:)";
		
		// draw bounding rect befor rotating text:
		Rect rect = new Rect();
		paint.getTextBounds(rotatedtext, 0, rotatedtext.length(), rect);
		canvas.translate(x,y);
		paint.setStyle(Paint.Style.FILL);
		
		canvas.drawText("Rotated helloandroid :)", 0, 0, paint);
		paint.setStyle(Paint.Style.STROKE);
		canvas.drawRect(rect, paint);
		
		canvas.translate(-x, -y);
		
		paint.setColor(Color.RED);
		canvas.rotate(-45, x + rect.exactCenterX(), y+rect.exactCenterY());
		paint.setStyle(Paint.Style.FILL);
		canvas.drawText(rotatedtext, x, y, paint);
		
		// draw a draw a thick dashed line in angle
		DashPathEffect dashPath = new DashPathEffect(new float[]{10, 40}, 1);
		paint.setPathEffect(dashPath);
		paint.setStrokeWidth(8);
		canvas.drawLine(0, 60, 320, 300, paint);
	}
	
	private class CanvasThread extends Thread{
		private SurfaceHolder surfaceHolder;
		private boolean isRun = false;
		
		public CanvasThread(SurfaceHolder holder) {
			surfaceHolder = holder;
		}
		public void setRunning(boolean run) {
			isRun = run;
		}
		
		@Override
		public void run() {
			Canvas c;
			while (isRun) {
				c = null;
				try {
					c = this.surfaceHolder.lockCanvas();
					synchronized(this.surfaceHolder) {
						CanvasDrawView.this.onDraw(c);
					}
				} finally {
					surfaceHolder.unlockCanvasAndPost(c);
				}
			}
		}
	}
}
