package com.example.appuidroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

public class CanvasDrawActivity extends Activity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.canvas_demo);
		
		CanvasDrawView drawview = (CanvasDrawView)findViewById(R.id.surfaceview01);
		drawview.setOnClickListener(new View.OnClickListener() {
			private boolean isDrawn = false;
			@Override
			public void onClick(View v) {
				if (!isDrawn) {
					CanvasDrawView drawview = (CanvasDrawView)findViewById(R.id.surfaceview01);
					drawview.startDrawImage();
					isDrawn = true;
				}
			}
		});
	}
}