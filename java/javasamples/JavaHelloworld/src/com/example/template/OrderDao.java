package com.example.template;

import java.util.ArrayList;
import java.util.List;

import com.example.util.ThreeTuple;

public class OrderDao extends BaseDao<OrderBean> {
	public OrderDao() {
		
	}
	
	@Override
	public void insertItems(List<OrderBean> items) {
		
	}
	
	@Override
	public void updateItems(List<OrderBean> items) {
		
	}
	
	@Override
	public List<String> fetchExistIds(List<String> ids) {
		return new ArrayList<String>();
	}

	@Override
	public void updateItemIds(ThreeTuple<String, String, String> idpairs) {
		
	}
}