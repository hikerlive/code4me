package com.example.appuidroid.graphics;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.example.appuidroid.R;

public class PanelView extends SurfaceView implements SurfaceHolder.Callback {
	
	private CanvasThread canvasThread;
	
	public PanelView(Context context, AttributeSet attrs) {
		super(context, attrs);
		getHolder().addCallback(this);
		canvasThread = new CanvasThread(getHolder(), this);
		setFocusable(true);
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
	}
	
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		canvasThread.setRunning(true);
		canvasThread.start();
	}
	
	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
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
	public void onDraw(Canvas canvas) {
		Paint paint = new Paint();
		
		Bitmap kangoo = BitmapFactory.decodeResource(getResources(), R.drawable.kangoo);
		canvas.drawColor(Color.BLACK);
		canvas.drawBitmap(kangoo, 10, 10, null);
	}
	
}
