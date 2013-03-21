package com.example.template;

import java.util.List;

import com.example.util.ThreeTuple;

public abstract class BaseDao<T>{
	public abstract void insertItems(List<T> items);
	public abstract void updateItems(List<T> items);
	public abstract void updateItemIds(ThreeTuple<String, String, String> idpairs);
	
	public abstract List<String> fetchExistIds(List<String> ids);
}