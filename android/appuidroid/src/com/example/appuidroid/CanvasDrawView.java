package com.example.appuidroid;

import android.R.color;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
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
