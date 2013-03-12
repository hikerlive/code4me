package com.example.appuidroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.LinearInterpolator;
import android.view.animation.RotateAnimation;
import android.widget.ImageView;

/**
 * 演示旋转动画的使用方法
 * @author Administrator
 */
public class RotateImageActivity extends Activity {
	private RotateAnimation animation;
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.rotate_image);
		
		/**
		animation = new RotateAnimation(0f, 360f, 
				Animation.RELATIVE_TO_SELF, 0.5f, 
				Animation.RELATIVE_TO_SELF, 0.5f
				);
		animation.setDuration(1000);
		*/

		Animation animation1 = AnimationUtils.loadAnimation(this, R.anim.rotate_image);
		LinearInterpolator lin = new LinearInterpolator();
		animation1.setInterpolator(lin);
		ImageView imageView = (ImageView)findViewById(R.id.register_loading);
		imageView.startAnimation(animation1);
	}
}