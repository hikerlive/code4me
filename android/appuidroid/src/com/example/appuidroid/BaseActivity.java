package com.example.appuidroid;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class BaseActivity extends Activity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}
	
	protected View onClick(int resid, View.OnClickListener listener) {
		View btn = (View)this.findViewById(resid);
		btn.setOnClickListener(listener);
		return btn;
	}
    
    public void onNavigate(Class<?> cls) {
    	if (null == cls) {
    		return;
    	}
    	
    	Intent intent = new Intent(this, cls);
    	startActivity(intent);
    }
}