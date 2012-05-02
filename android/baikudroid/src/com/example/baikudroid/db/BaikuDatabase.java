package com.example.baikudroid.db;

import com.example.baikudroid.data.Tweet;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

public class BaikuDatabase {
	private static final String TAG = "BaikuDatabase";
	private static final int DATABASE_VERSION = 1;
	private static final String DATABASE_NAME = "baikud_db";
	
	private static BaikuDatabase instance = null;
	private static DatabaseHelper mOpenHelper = null;
	private Context mContext = null;

	private static class DatabaseHelper extends SQLiteOpenHelper {
		public DatabaseHelper(Context context, String name, 
				CursorFactory factory, int version) {
			super(context, name, factory, version);
		}
		
		public DatabaseHelper(Context context, String name) {
			this(context, name, null, DATABASE_VERSION);
		}
		
		@Override
		public void onCreate(SQLiteDatabase db) {
			Log.d(TAG, "onCreate");
			db.execSQL(ContentTable.CREATE_TABLE);
		}
		
		@Override
		public synchronized void close() {
			Log.d(TAG, "Close Database");
			super.close();
		}
		
		@Override
		public void onOpen(SQLiteDatabase db) {
			Log.d(TAG, "Open Database");
			super.onOpen(db);
			// dropAllTables(db);
		}
		
		@Override
		public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
			Log.d(TAG, "Upgrade Database");
			dropAllTables(db);
		}
		
		private void dropAllTables(SQLiteDatabase db) {
			db.execSQL("DROP TABLE IF EXISTS " + ContentTable.TABLE_NAME);
		}
	}
	
	public static synchronized BaikuDatabase getInstance(Context context) {
		Log.i(TAG, "getInstance()");
		if (null == instance) {
			return new BaikuDatabase(context);
		}
		return instance;
	}
	
	private BaikuDatabase(Context context) {
		Log.i(TAG, "BaikuDatabase()");
		mContext = context;
		mOpenHelper = new DatabaseHelper(context, DATABASE_NAME);
	}
	
	public void close() {
		Log.i(TAG, "close()");
		if (null != instance) {
			mOpenHelper.close();
			instance = null;
		}
	}
	
	public Cursor fetchAllTweets(String userid, int type) {
		Log.i(TAG, "fetchAllTweets()");
		SQLiteDatabase db = mOpenHelper.getReadableDatabase();
		return db.query(ContentTable.TABLE_NAME, ContentTable.TABLE_COLUMNS,
				null, null, null, null, null);
	}
	
	public long insertTweet(Tweet tweet) {
		Log.i(TAG, "insertTweet()");
		SQLiteDatabase db = mOpenHelper.getWritableDatabase();
		
		if (isExists(tweet.id)) {
			Log.w(TAG, tweet.id + "is exists.");
			return -1;
		}
		
		ContentValues values = makeTweetValues(tweet);
		long id = db.insert(ContentTable.TABLE_NAME, null, values);
		if (-1 == id) {
			Log.e(TAG, "can't insert the tweet :" + tweet.toString());
		}
		
		return id;
	}
	
	public boolean isExists(String tweetId) {
		SQLiteDatabase db = mOpenHelper.getWritableDatabase();
		boolean result = false;
		Cursor cursor = db.query(ContentTable.TABLE_NAME,
				new String[] { ContentTable._ID }, ContentTable._ID + " =? ", 
				new String[] {tweetId }, null, null, null);
		
		if (cursor != null && cursor.getCount() > 0) {
			result = true;
		}
		cursor.close();
		return result;
	}
	
	private ContentValues makeTweetValues(Tweet tweet) {
		Log.i(TAG, "makeTweetValues()");
		ContentValues values = new ContentValues();
		values.put(ContentTable._ID, tweet.id);
		values.put(ContentTable.MSG_ID, tweet.msgid);
		values.put(ContentTable.SHARE_TYPE, tweet.shareType);
		values.put(ContentTable.SENDER, tweet.sender);
		values.put(ContentTable.SCREEN_NAME, tweet.screenName);
		values.put(ContentTable.ICON_URL, tweet.iconUrl);
		values.put(ContentTable.CONTENTS, tweet.contents);
		values.put(ContentTable.IS_FAVORITED, tweet.isFavorited);
		return values;
	}
}