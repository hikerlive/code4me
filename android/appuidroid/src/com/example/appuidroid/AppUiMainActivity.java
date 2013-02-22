package com.example.appuidroid;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import android.app.ListActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class AppUiMainActivity extends ListActivity{
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setListAdapter(new SimpleAdapter(this, getData(),
				android.R.layout.simple_list_item_1,
				new String[]{"title"},
				new int[]{android.R.id.text1}));
		getListView().setTextFilterEnabled(true);
	}
	
	private List getData() {
		List<Map> list = new ArrayList<Map>();
		return list;
	}
	
	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		
	}
}