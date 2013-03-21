package com.example.template;

import java.util.List;

public class OrderBean extends BaseBean<OrderBean>{
	public OrderBean(String jsonResponse) {
		super(jsonResponse);
	}

	public String id;
	
	public void print() {
		System.out.print(OrderBean.class.getName());
	}

	@Override
	public List<OrderBean> asJSONObject(String jarray) {
		return null;
	}
	
	@Override
	public String getId() {
		return id;
	}
}