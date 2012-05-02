package com.example.baikudroid.ui.module;

import com.example.baikudroid.R;
import com.example.baikudroid.db.ContentTable;

import android.widget.CursorAdapter;
import android.widget.LinearLayout;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.ImageView;
import android.content.Context;
import android.database.Cursor;
import android.text.TextUtils;
import android.util.Log;
import android.view.ViewGroup;
import android.view.View;
import android.view.LayoutInflater;

public class TweetCursorAdapter extends CursorAdapter implements TweetAdapter {
	
	private static final String TAG = "TweetCursorAdapter";
	
	private LayoutInflater mInflater;
	private Context mContext;
	
	private int mUserTextColumn;
	private int mTextColumn;
	private int mProfileImageUrlColumn;
	private int mThumbnailPic;
	private int mMiddlePic;
	private int mOriginalPic;
	private int mCreateAtColumn;
	private int mSourceColumn;
	private int mInReplyToScreenName;
	private int mFavorited;
	
	private StringBuilder mMetaBuilder;
	
	public TweetCursorAdapter(Context context, Cursor cursor) {
		super(context, cursor);
		mContext = context;
		Log.i(TAG, "TweetCursorAdapter()");
		if (mContext != null) {
			mInflater = LayoutInflater.from(context);
		}
		
		if (cursor != null) {
			mUserTextColumn = cursor.getColumnIndexOrThrow(ContentTable.SCREEN_NAME);
			mProfileImageUrlColumn = cursor.getColumnIndex(ContentTable.ICON_URL);
			mTextColumn = cursor.getColumnIndex(ContentTable.CONTENTS);
			mFavorited = cursor.getColumnIndex(ContentTable.IS_FAVORITED);
		}
		
		mMetaBuilder = new StringBuilder();
	}
	
	private static class ViewHolder {
		public LinearLayout tweetLayout;
		public TextView tweetUserText;
		public TextView tweetText;
		public FrameLayout profileLayout;
		public ImageView profileImage;
		public TextView metaText;
		public ImageView fav;
		public ImageView hasImage;
	}
	
	@Override
	public View newView(Context context, Cursor cursor, ViewGroup parent) {
		Log.i(TAG, "newView()");
		View view = mInflater.inflate(R.layout.tweet, parent, false);
		
		TweetCursorAdapter.ViewHolder holder = new ViewHolder();
		holder.tweetLayout = (LinearLayout)view.findViewById(R.id.tweet_layout);
		holder.profileLayout = (FrameLayout)view.findViewById(R.id.profile_layout);
		holder.profileImage = (ImageView)view.findViewById(R.id.profile_image);
		holder.tweetUserText = (TextView)view.findViewById(R.id.tweet_user_text);
		holder.hasImage = (ImageView)view.findViewById(R.id.tweet_has_image);
		holder.tweetText = (TextView)view.findViewById(R.id.tweet_text);
		holder.metaText = (TextView)view.findViewById(R.id.tweet_meta_text);
		holder.fav = (ImageView)view.findViewById(R.id.tweet_fav);
		
		view.setTag(holder);
		return view;
	}
	
	@Override
	public void bindView(View view, Context context, Cursor cursor) {
		Log.i(TAG, "bindView()");
		TweetCursorAdapter.ViewHolder holder = (TweetCursorAdapter.ViewHolder)view.getTag();
		
		holder.tweetUserText.setText(cursor.getString(mUserTextColumn));
		holder.tweetText.setText(cursor.getString(mTextColumn));
		
		// showProfileImage
		String profileImageUrl = cursor.getString(mProfileImageUrlColumn);
		if (!TextUtils.isEmpty(profileImageUrl)) {
			holder.profileLayout.setVisibility(View.VISIBLE);
			// SimpleImageLoader.display(holder.profileImage, profileImageUrl);
		} else {
			holder.profileLayout.setVisibility(View.GONE);
		}
		
		// isFavorited
		if (cursor.getString(mFavorited).equals("true")) {
			holder.fav.setVisibility(View.VISIBLE);
		} else {
			holder.fav.setVisibility(View.GONE);
		}
		
		/*
		// hasImage
		if (!TextUtils.isEmpty(cursor.getString(mThumbnailPic))) {
			holder.hasImage.setVisibility(View.VISIBLE);
		} else {
			holder.hasImage.setVisibility(View.GONE);
		}
		*/
		
		// default
		holder.tweetLayout.setBackgroundResource(android.R.drawable.list_selector_background);
		holder.profileLayout.setBackgroundResource(android.R.color.transparent);
	}
	
	@Override
	public void refresh(){
		getCursor().requery();
	}
}
