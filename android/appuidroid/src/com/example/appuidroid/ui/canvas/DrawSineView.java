package com.example.appuidroid.ui.canvas;

import android.app.Activity;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;

import com.example.appuidroid.R;

public class DrawSineView extends Activity {
	private Button btn1, btn2;
	private SurfaceView sfv;
	private SurfaceHolder sfvholder;
	
	int Y_axis[], centerY, oldX, oldY, currentX;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.canvas_sine);
		
		btn1 = (Button) findViewById(R.id.btn1);
		btn2 = (Button) findViewById(R.id.btn2);
		btn1.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				sineDraw(Y_axis.length - 1);
			}
		});
		
		sfv = (SurfaceView) findViewById(R.id.surfaceview1);
		sfvholder = sfv.getHolder();
		
		centerY = (getWindowManager().getDefaultDisplay().getHeight()- sfv.getTop()) /2;
		Y_axis = new int[getWindowManager().getDefaultDisplay().getWidth()];
		for (int i=1; i<Y_axis.length; i++) {
			Y_axis[i-1] = centerY - (int)(100 * Math.sin(i*2*Math.PI/180));
		}
	}
	
	private void sineDraw(int length){
		if (length == 0) {
			oldX = 0;
		}
		Canvas canvas = sfvholder.lockCanvas(
				new Rect(oldX, 0, oldX+length, getWindowManager().getDefaultDisplay().getHeight())
				); // 获得画布
		Paint paint = new Paint(); // 获得画笔
		paint.setColor(Color.GREEN); // 画笔为绿色
		paint.setStrokeWidth(2); // 设置画笔粗细
		
		int y;
		for (int i = oldX + 1; i<length; i++) {
			y = Y_axis[i-1];
			canvas.drawLine(oldX, oldY, i, y, paint); // 画线
			oldX = i;
			oldY = y;
		}
		sfvholder.unlockCanvasAndPost(canvas); // 解锁画布
	}
	
	private void clearDraw() {
		Canvas canvas = sfvholder.lockCanvas(null);
		canvas.drawColor(Color.BLACK); // 清除画布
		sfvholder.unlockCanvasAndPost(canvas);
	}
}