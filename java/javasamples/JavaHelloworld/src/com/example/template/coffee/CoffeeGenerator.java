package com.example.template.coffee;

import java.util.Iterator;
import java.util.Random;

public class CoffeeGenerator implements Generator<Coffee>, Iterator<Coffee> {
	private Class[] types = {Latte.class, Mocha.class};
	private static Random rand = new Random(47);
	private int size = 0;
	
	public CoffeeGenerator() {
	}
	
	public CoffeeGenerator(int sz) {
		this.size = sz;
	}
	
	@Override
	public Coffee next() {
		try {
			return (Coffee)
				types[rand.nextInt(types.length)].newInstance();
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}
	
	class CoffeeIterator implements Iterator<Coffee> {
		int count = size;
		@Override
		public boolean hasNext() {
			return count > 0;
		}

		@Override
		public void remove() {
			throw new UnsupportedOperationException();
		}

		@Override
		public Coffee next() {
			count --;
			return CoffeeGenerator.this.next();
		}
	};
	
	public Iterator<Coffee> iterator() {
		return new CoffeeIterator();
	}
	
	@Override
	public boolean hasNext() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void remove() {
		// TODO Auto-generated method stub
		
	}
	
	public static void main(String[] args) {
		CoffeeGenerator gen = new CoffeeGenerator();
		for (int i=0; i<2; i++) {
			System.out.print(gen.next());
		}
	}

}
