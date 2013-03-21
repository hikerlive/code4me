package com.example.template;

import java.util.List;

public abstract class BaseBean<T> {
	public BaseBean() {
		
	}
	
	public BaseBean(String jsonResponse) {
		
	}
	
	public abstract List<T> asJSONObject(String jarray); 
	public abstract String getId();
	
	public static String getName() {
		return "";
	}
}