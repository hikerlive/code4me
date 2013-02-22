package com.example.appuidroid;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.example.appuidroid.asynchttp.AsyncHttpActivity;
import com.example.appuidroid.graphics.DrawActivity;
import com.example.appuidroid.graphics.PanelActivity;

public class AppUiGuideActivity extends BaseActivity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        onClick(R.id.demo_qqlogin, new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(AppUiGuideActivity.this, QQLoginActivity.class);
				startActivity(intent);
			}
		});
        onClick(R.id.demo_qqcontactdial, new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent(AppUiGuideActivity.this, QQContactActivity.class);
        		startActivity(intent);
        	}
        });
        onClick(R.id.demo_sensormonitor, new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent(AppUiGuideActivity.this, SensorMonitorActivity.class);
        		startActivity(intent);
        	}
        });    
        onClick(R.id.demo_viewpager, new View.OnClickListener() {
        	@Override
        	public void onClick(View v) {
        		Intent intent = new Intent(AppUiGuideActivity.this, ViewPagerActivity.class);
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
        		Intent intent = new Intent(AppUiGuideActivity.this, PathMainActivity.class);
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
        onClick(R.id.demo_contactlist, new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(AppUiGuideActivity.this, LetterListActivity.class);
				startActivity(intent);
			}
		});
        onClick(R.id.demo_custom_panel, new View.OnClickListener() {
        	@Override
			public void onClick(View v) {
				Intent intent = new Intent(AppUiGuideActivity.this, PanelActivity.class);
				startActivity(intent);
			}
        });
        onClick(R.id.demo_custom_draw, new View.OnClickListener() {
        	@Override
			public void onClick(View v) {
				Intent intent = new Intent(AppUiGuideActivity.this, DrawActivity.class);
				startActivity(intent);
			}
        });
        onClick(R.id.demo_canvas, new View.OnClickListener() {
        	@Override
			public void onClick(View v) {
				Intent intent = new Intent(AppUiGuideActivity.this, CanvasDrawActivity.class);
				startActivity(intent);
			}
        });
        onClick(R.id.demo_rotate_image, new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				onNavigate(RotateImageActivity.class);
			}
		});
        onClick(R.id.demo_download_image, new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				onNavigate(DownloadImageActivity.class);
			}
		});
        onClick(R.id.demo_sms_thread, new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				onNavigate(SMSThreadActivity.class);
			}
		});
        onClick(R.id.demo_async_http, new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				onNavigate(AsyncHttpActivity.class);
			}
		});
        onClick(R.id.demo_jni_caller, new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				onNavigate(JniCallerActivity.class);
			}
		});
    }
}