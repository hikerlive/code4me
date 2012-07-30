package com.example.appuidroid;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class AppUiGuideActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        Button btn1 = (Button)findViewById(R.id.demo_qqlogin);
        btn1.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent("com.example.appuidroid.QQLOGIN");
				startActivity(intent);
			}
		});
        
        Button btn2 = (Button)findViewById(R.id.demo_qqcontactdial);
        btn2.setOnClickListener(new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent("com.example.appuidroid.QQCONTACTDIAL");
        		startActivity(intent);
        	}
        });
        
        Button btn3 = (Button)findViewById(R.id.demo_sensormonitor);
        btn3.setOnClickListener(new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent("com.example.appuidroid.SENSORDEMO");
        		startActivity(intent);
        	}
        });    
        
        Button btn4 = (Button)findViewById(R.id.demo_viewpager);
        btn4.setOnClickListener(new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent("com.example.appuidroid.VIEWPAGER");
        		startActivity(intent);
        	}
        });
        
        Button btn5 = (Button)findViewById(R.id.demo_pathmain);
        btn5.setOnClickListener(new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent("com.example.appuidroid.PATHMAIN");
        		startActivity(intent);
        	}
        });
    }
}