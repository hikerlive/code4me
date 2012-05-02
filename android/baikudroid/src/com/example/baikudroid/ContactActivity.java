package com.example.baikudroid;

import java.util.LinkedList;
import java.util.List;

import android.app.Activity;
import android.content.ContentResolver;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.provider.ContactsContract.PhoneLookup;

public class ContactActivity extends Activity {
	
	public class Contact {
		public String name = "";
		public String phoneNumber = "";
		public String email = "";
		public String address = "";
	};
	
	private List<Contact> contactList = new LinkedList<Contact>();
	private static final String LAUNCH_ACTION = "com.example.baikudroid.CONTACT";

	protected void onCreate(Bundle SaveInstanceState) {
		super.onCreate(SaveInstanceState);
		
		readContact();
	}
	
	public static Intent createIntent() {
		Intent intent = new Intent(LAUNCH_ACTION);
		intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		return intent;
	}
	
	protected void readContact() {
		ContentResolver cr = getContentResolver();
		
		// ȡ�õ绰���п�ʼһ��Ĺ�꣬������moveToNext()
		Cursor cursor = cr.query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);
		while (cursor.moveToNext()) {
			Contact obj = new Contact();
			// ȡ����ϵ�˵���������
			int index = cursor.getColumnIndex(PhoneLookup.DISPLAY_NAME);
			obj.name = cursor.getString(index);
			
			// ȡ����ϵ�˵�ID����ֵ
			String contactId = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts._ID));
			String selection = ContactsContract.CommonDataKinds.Phone.CONTACT_ID + " = " + contactId;
			Cursor phone = cr.query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI, null, selection, null, null);
			
			// һ���˿����м�������
			while (phone.moveToNext()) {
				obj.phoneNumber = phone.getString(phone.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));
				obj.email = phone.getString(phone.getColumnIndex(ContactsContract.CommonDataKinds.Email.DISPLAY_NAME));
			}
			contactList.add(obj);
			phone.close();
		}
		cursor.close();
	}
}