package com.example.template;

import java.util.ArrayList;
import java.util.List;

import com.example.util.ThreeTuple;

public class SyncGenericClass<T extends BaseBean<T>, X extends BaseDao<T>> {
	private X dao;
	private Class tclass;
	
	public void setBeanDao(X x) {
		this.dao = x;
	}
	
	public SyncGenericClass(Class tcls) {
		this.tclass = tcls; 
	}
	
	public SyncGenericClass(Class tcls, X x) {
		this.tclass = tcls;
		this.dao = x;
	}
	
	public void _disposeHttpDownload(String jarray) {
		// items.add(new T()); // Error: Cannot instantiable the type T，需要一个类生成器。
		BaseBean t = null;
		try {
			t = (BaseBean)tclass.newInstance();
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
		
		List<T> beans = t.asJSONObject(jarray);
		if (beans.isEmpty()) {
			return;
		}
		t = null;
		
		List<String> ids = new ArrayList<String>();
		for (T one : beans) {
			ids.add(one.getId());
		}
		List<String> localids = dao.fetchExistIds(ids);
		List<T> exists = new ArrayList<T>();
		List<T> inserts = new ArrayList<T>();
		for (T one : beans) {
			if (localids.contains(one.getId())) {
				exists.add(one);
			} else {
				inserts.add(one);
			}
		}
		
		if (!inserts.isEmpty()) {
			dao.insertItems(inserts);
		}
		if (!exists.isEmpty()) {
			dao.updateItems(exists);
		}

		ids = null;
		localids = null;
		exists = null;
		inserts = null;
	}

	public void _disposeHttpUpload(String jresults) {
		// 问题，如何解决其它数据情况的匹配。
		this.dao.updateItemIds(
				new ThreeTuple<String, String, String>("01", "01", "110"));
	}
}