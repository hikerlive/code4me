package com.example.appuidroid.graphics;

import android.graphics.Canvas;
import android.view.SurfaceHolder;

public class CanvasThread extends Thread{
	private SurfaceHolder surfaceHolder;
	private PanelView panel;
	private boolean isRun = false;
	
	public CanvasThread(SurfaceHolder surfaceHolder, PanelView panel) {
		this.surfaceHolder = surfaceHolder;
		this.panel = panel;
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
				c = surfaceHolder.lockCanvas();
				synchronized(surfaceHolder) {
					this.panel.onDraw(c);
				}
			} finally {
				if (c != null) {
					this.surfaceHolder.unlockCanvasAndPost(c);
				}
			}
		}
	}

}
