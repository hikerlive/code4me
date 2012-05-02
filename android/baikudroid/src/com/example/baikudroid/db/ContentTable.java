package com.example.baikudroid.db;

import android.provider.BaseColumns;

public final class ContentTable implements BaseColumns {
	public static final String TAG = "ContentTable";
	
	public static final int TYPE_HOME = 1;
	public static final int TYPE_MENTION = 2;
	public static final int TYPE_USER = 3;
	public static final int TYPE_FAVORITE = 4;
	public static final int TYPE_BROWSE = 5;
	
	public static final String TABLE_NAME = "content";
	public static final String MSG_ID = "msgid";
	public static final String SHARE_TYPE = "share_type";
	public static final String SENDER = "sender";
	public static final String SCREEN_NAME = "screen_name";
	public static final String ICON_URL = "icon";
	public static final String CONTENTS = "contents";
	public static final String IS_FAVORITED = "is_favorited";
	
	public static final String[] TABLE_COLUMNS = new String[] {_ID,
		MSG_ID, SHARE_TYPE, SENDER, SCREEN_NAME, ICON_URL, CONTENTS, IS_FAVORITED};
	
	public static final String CREATE_TABLE = 
		"CREATE TABLE " + TABLE_NAME  + "("
		+ _ID + " text not null, "
		+ MSG_ID + " text not null, "
		+ SHARE_TYPE + " text not null, "
		+ SENDER + " text not null, "
		+ SCREEN_NAME + " text not null, "
		+ ICON_URL + " text, "
		+ CONTENTS + " text, "
		+ IS_FAVORITED + " text, "
		+ "PRIMARY KEY (" + MSG_ID + "," + SHARE_TYPE + "," + SENDER + "))";
}