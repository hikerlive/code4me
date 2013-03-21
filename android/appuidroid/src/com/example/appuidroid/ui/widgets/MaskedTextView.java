package com.example.appuidroid.ui.widgets;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Shader;
import android.graphics.Shader.TileMode;
import android.util.AttributeSet;
import android.view.View;

public class MaskedTextView extends View {
	private Paint txtPaint;
	private Shader shader;
	private float dx = 50;
	private long lastTime = System.currentTimeMillis(); // 获取当前时间毫秒数
	private boolean start = false; // 是否开启遮罩效果
	
	public MaskedTextView(Context context) {
		super(context);
		init();
	}
	
	public MaskedTextView(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}
	
	public MaskedTextView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init();
	}
	
	private void init() {
		txtPaint = new Paint(); // 初始化画笔
		txtPaint.setColor(Color.GRAY);  // 设置画笔颜色
		txtPaint.setAntiAlias(true); // 抗锯齿效果
		txtPaint.setTextSize(26);
		
		// 创建线性渐变效果作为遮罩效果
		shader = new LinearGradient(0, 0, 200, 0, 
				new int[] {Color.argb(255, 120, 120, 120), Color.argb(255, 120, 120, 120), Color.WHITE}, 
				new float[]{0, 0.7f, 1}, TileMode.MIRROR);
		txtPaint.setShader(shader);
	}
	
	public void setStart(boolean start) {
		this.start = start;
		invalidate();
	}
	
	@Override
	public void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		
		// 获取系统当前时间毫秒数
		long now = System.currentTimeMillis();
		// 计算时间差并缩小4.5倍作为位移量
		float elapsed = (now - lastTime) / 4.5f;
		// 累加位移量
		dx += elapsed;

		// 创建变换矩阵
		Matrix matrix = new Matrix();
		if (start) {
			// 对变换矩阵进行平移dx变换
			matrix.setTranslate(dx, 0);
			invalidate();
		} else {
			matrix.setTranslate(0, 0);
		}
		
		// 对遮罩效果进行变换处理
		shader.setLocalMatrix(matrix);
		// 绘制文本
		canvas.drawText("MaskedText-文本遮罩效果", 0, 25, txtPaint);
		// 记录最后绘制文本的时间
		lastTime = now;
	}
}