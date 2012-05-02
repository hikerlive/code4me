package com.example.baikudroid;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

public class IntentActivity extends ListActivity {
	private static final String LAUNCH_ACTION1="com.example.baikudroid.INTENT";
	private static final String LAUNCH_ACTION2="com.example.baikudroid.SUBINTENT";
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		// ����ListAdapter
		setListAdapter(new SimpleAdapter(this, getData(),
			android.R.layout.simple_list_item_1, new String[]{"title"}, new int[]{android.R.id.text1}));
		getListView().setTextFilterEnabled(true);
		
		// ע��һ���㲥
		super.registerReceiver(new BroadcastReceiver() {
			@Override
			public void onReceive(Context context, Intent intent) {
				Toast.makeText(context, "��Broadcast��Intent Filterֵֻ������Action", Toast.LENGTH_SHORT).show();
			}
		}, new IntentFilter(LAUNCH_ACTION1));
	}
	
	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		Intent intent = (Intent)getData().get(position).get("intent");
		Boolean isActivity = (Boolean)getData().get(position).get("isActivity");
		if (isActivity) {
			startActivity(intent); 
		}else {
			 sendBroadcast(intent);
		}
	}
	
	private List<Map<String, Object>> getData() {
		List<Map<String, Object>> data = new ArrayList<Map<String, Object>>();
		addItem(data, "no action", new Intent(), true);
		addItem(data, "custom action", new Intent(LAUNCH_ACTION2), true);
		addItem(data, "broadcast action", new Intent(LAUNCH_ACTION1), false);
		return data;
	}
	
	private void addItem(List<Map<String, Object>> data, String name, Intent intent, boolean isActivity) {
		Map<String, Object> temp = new HashMap<String, Object>();
		temp.put("title", name);
		temp.put("intent", intent);
		temp.put("isActivity", isActivity);
		data.add(temp);
	}
	
	public static Intent createIntent() {
		Intent intent = new Intent(LAUNCH_ACTION1);
		intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		return intent;
	}
};