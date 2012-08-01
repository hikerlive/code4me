package com.example.appuidroid;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class AppUiGuideActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        onClick(R.id.demo_qqlogin, new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent("com.example.appuidroid.QQLOGIN");
				startActivity(intent);
			}
		});
        onClick(R.id.demo_qqcontactdial, new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent("com.example.appuidroid.QQCONTACTDIAL");
        		startActivity(intent);
        	}
        });
        onClick(R.id.demo_sensormonitor, new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent("com.example.appuidroid.SENSORDEMO");
        		startActivity(intent);
        	}
        });    
        onClick(R.id.demo_viewpager, new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent("com.example.appuidroid.VIEWPAGER");
        		startActivity(intent);
        	}
        });
        onClick(R.id.demo_scrolltab, new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent(AppUiGuideActivity.this, ScrollTabActivity.class);
        		startActivity(intent);
        	}
        });
        onClick(R.id.demo_pathmain, new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent("com.example.appuidroid.PATHMAIN");
        		startActivity(intent);
        	}
        });
        onClick(R.id.demo_wechatguide, new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(AppUiGuideActivity.this, WeChatGuideActivity.class);
				startActivity(intent);
			}
		});
    }
    
    private View onClick(int resId, View.OnClickListener listener) {
    	View view = (View) findViewById(resId);
    	view.setOnClickListener(listener);
    	return view;
    }
}