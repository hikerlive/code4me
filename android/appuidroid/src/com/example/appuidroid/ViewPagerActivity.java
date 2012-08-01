package com.example.appuidroid;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.os.Bundle;
import android.os.Parcelable;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.util.DisplayMetrics;
import android.view.LayoutInflater;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.ImageView;
import android.widget.TextView;

public class ViewPagerActivity extends Activity{
	private ViewPager mViewPager;
	private ImageView mCursor;
	private TextView mTab1;
	private TextView mTab2;
	private TextView mTab3;
	
	private List<View> views;
	private int offset = 0;
	private int currIndex = 0;
	private int bitmapWidth;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.page_main);
		
		mCursor = (ImageView)findViewById(R.id.cursor);
		bitmapWidth = BitmapFactory.decodeResource(getResources(), 
				R.drawable.viewpager_tab_selected).getWidth();
		DisplayMetrics dm = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(dm);
		
		int screenW = dm.widthPixels;
		offset = (screenW/3-bitmapWidth)/2;
		Matrix matrix = new Matrix();
		matrix.postTranslate(offset, 0);
		mCursor.setImageMatrix(matrix);
		
		mTab1 = (TextView)findViewById(R.id.text1);
		mTab1.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				mViewPager.setCurrentItem(0);
			}
		});
		mTab2 = (TextView)findViewById(R.id.text2);
		mTab2.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				mViewPager.setCurrentItem(1);
			}
		});
		mTab3 = (TextView)findViewById(R.id.text3);
		mTab3.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				mViewPager.setCurrentItem(2);
			}
		});	
		
		mViewPager = (ViewPager)findViewById(R.id.vPager);
		views = new ArrayList<View>();
		LayoutInflater inflater = getLayoutInflater();
		views.add(inflater.inflate(R.layout.page_one, null));
		views.add(inflater.inflate(R.layout.page_two, null));
		views.add(inflater.inflate(R.layout.page_three, null));
		
		mViewPager.setAdapter(new MyPagerAdapter(views));
		mViewPager.setCurrentItem(0);
		mViewPager.setOnPageChangeListener(new MyOnPageChangeListener());
	}
	
	public class MyPagerAdapter extends PagerAdapter{
		public List<View> mListViews;
		public MyPagerAdapter(List<View> list) {
			this.mListViews = list;
		}
	
		@Override
		public void destroyItem(View arg0, int arg1, Object arg2) {
			((ViewPager) arg0).removeView(mListViews.get(arg1));
		}
		
		@Override
		public void startUpdate(View arg0) {
			
		}
		
		@Override
		public void finishUpdate(View arg0) {
			
		}
		
		@Override
		public int getCount() {
			return mListViews.size();
		}
		
		@Override
		public Object instantiateItem(View arg0, int arg1) {
			((ViewPager) arg0).addView(mListViews.get(arg1), 0);
			return mListViews.get(arg1);
		}
		
		@Override
		public boolean isViewFromObject(View arg0, Object arg1) {
			return arg0 == (arg1);
		}
		
		@Override
		public void restoreState(Parcelable arg0, ClassLoader arg1) {
			
		}
		
		@Override
		public Parcelable saveState() {
			return null;
		}
	}

	public class MyOnPageChangeListener implements OnPageChangeListener {
		int one = offset*2 + bitmapWidth;
		int two = one*2;
		
		@Override
		public void onPageSelected(int arg0) {
			Animation animation = null;
			switch (arg0) {
			case 0: {
				if (currIndex == 1) {
					animation = new TranslateAnimation(one, 0, 0, 0);
				}
				else if (currIndex == 2) {
					animation = new TranslateAnimation(two, 0, 0, 0);
				}
				}break;
			case 1: {
				if (currIndex == 0) {
					animation = new TranslateAnimation(offset, one, 0, 0);
				} else if (currIndex == 2) {
					animation = new TranslateAnimation(two, one, 0, 0);
				}
				}break;
			case 2: {
				if (currIndex == 0) {
					animation = new TranslateAnimation(offset, two, 0, 0);
				} else if (currIndex == 1) {
					animation = new TranslateAnimation(one, two, 0, 0);
				}
				}break;
			}
			currIndex = arg0;
			animation.setFillAfter(true);
			animation.setDuration(300);
			mCursor.startAnimation(animation);
		}
		
		@Override
		public void onPageScrolled(int arg0, float arg1, int arg2) {
			
		}
		
		@Override
		public void onPageScrollStateChanged(int arg0) {
			
		}
	}
}