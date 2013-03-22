package com.example.appuidroid.ui.canvas;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Rect;
import android.os.Bundle;
import android.view.View;

import com.example.appuidroid.R;

/**
 * 演示说明
 *  1.演示Activity的2D基本绘制技能，如矩形、文字、图片。
 */
public class HelloWorldActivity extends Activity {
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		HelloWorldView view = new HelloWorldView(this);
		setContentView(view);
	}
	
	public class HelloWorldView extends View {
		public HelloWorldView(Context context) {
			super(context);
		}
		
		@Override
		protected void onDraw(Canvas canvas) {
			super.onDraw(canvas);
			
			// 绘制实心矩形
			Paint paint = new Paint();
			paint.setColor(Color.BLUE);
			paint.setStyle(Style.FILL);
			canvas.drawRect(new Rect(0, 0, getWidth(), getHeight()), paint);
			
			// 绘制空心矩形
			paint.setColor(Color.RED);
			paint.setStyle(Style.STROKE);
			canvas.drawRect(new Rect(10, 10, 100, 30), paint);
			
			// 绘文字
			paint.setColor(Color.GREEN);
			canvas.drawText("Hello World", 10, 50, paint);
			
			// 绘图
			Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.ic_launcher);
			canvas.drawBitmap(bitmap, 10, 60, paint);
		}
	}
}