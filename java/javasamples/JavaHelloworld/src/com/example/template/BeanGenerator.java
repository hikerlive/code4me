package com.example.template;

import com.example.template.coffee.Generator;

public class BeanGenerator implements Generator<BaseBean> {
	private Class[] types = { OrderBean.class };
	
	public BeanGenerator() {}
	
	@Override
	public BaseBean next() {
		try {
			return (BaseBean) types[0].newInstance();
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}
	
}