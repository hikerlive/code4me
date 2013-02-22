package com.example.appuidroid.contact;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.Context;
import android.database.Cursor;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds.Email;
import android.provider.ContactsContract.CommonDataKinds.Phone;

import com.example.appuidroid.utils.StringUtil;

public class SystemContactDao {
	private Context context;
	public SystemContactDao(Context context) {
		this.context = context;
	}
	
	private List<Contact> loadSystemContact1() {
		List<Contact> infos = new ArrayList<Contact>();
		
		ContentResolver cr = context.getContentResolver();
		Cursor cursor = cr.query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);
		if (null != cursor) { 
			if (cursor.moveToNext()) {
				do {
					Contact ii = new Contact();
					ii.id = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts._ID));
					ii.name = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
					ii.photo = ContentUris.withAppendedId(ContactsContract.Contacts.CONTENT_URI, Long.parseLong(ii.id)).toString();
		
					int hasNumbers = cursor.getInt(cursor.getColumnIndex(ContactsContract.Contacts.HAS_PHONE_NUMBER));
					if (hasNumbers > 0) {
						Cursor cursorPhone = cr.query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI, null, ContactsContract.CommonDataKinds.Phone.CONTACT_ID + " = ?", new String[] {ii.id}, null);
						if (cursorPhone != null) {
							while (cursorPhone.moveToNext()) {
								String type = cursorPhone.getString(cursorPhone.getColumnIndex(ContactsContract.CommonDataKinds.Phone.TYPE));
								String number = cursorPhone.getString(cursorPhone.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));
								if (type.equals("1") && ii.tel.equals("")) {
									ii.tel = number;
								}
								else if (type.equals("2") && ii.mobile.equals("")) {
									ii.mobile = number;
								}
							}
							cursorPhone.close();
						}
					}
		
					Cursor cursorEmail = cr.query(ContactsContract.CommonDataKinds.Email.CONTENT_URI, null, ContactsContract.CommonDataKinds.Email.CONTACT_ID + " = ?", new String[] {ii.id}, null);
					if (cursorEmail != null) {
						while (cursorEmail.moveToNext()) {
							ii.email = cursorEmail.getString(cursorEmail.getColumnIndex(ContactsContract.CommonDataKinds.Email.DATA));
						}
						cursorEmail.close();
					}
					infos.add(ii);
				} while (cursor.moveToNext()); 
			} 
			cursor.close();
		}
		
		return infos;
	}
	
	private List<Contact> loadSystemContact2() {
		
		ArrayList<Contact> infos = new ArrayList<Contact>();
		ArrayList<String> ids = new ArrayList<String>();
		ArrayList<String> hasids = new ArrayList<String>();
		
		Map<Integer, Contact> mapinfos = new HashMap<Integer, Contact>();
		
		// step1: load contact id, name, photo file id
		ContentResolver cr = context.getContentResolver();
		Cursor cursor = cr.query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);
		if (null != cursor) {
			if (cursor.moveToNext()) {
				do {
					Contact info = new Contact();
					int contactid = cursor.getInt(cursor.getColumnIndex(ContactsContract.Contacts._ID));
					info.id = Integer.toString(contactid);
					info.name = cursor.getString(cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
					info.photo = ContentUris.withAppendedId(ContactsContract.Contacts.CONTENT_URI, Long.parseLong(info.id)).toString();
					int hasNumber = cursor.getInt(cursor.getColumnIndex(ContactsContract.Contacts.HAS_PHONE_NUMBER));
					if (hasNumber > 0) {
						hasids.add(info.id);
					}
					ids.add(info.id);
					infos.add(info);
					
					mapinfos.put(contactid, info);
				} while (cursor.moveToNext());
			}
			cursor.close();
		}
		
		// step2: read phone_number which has phone_number
		if (!hasids.isEmpty()) {
			String whereSelection = String.format( "%s IN (%s)",
					ContactsContract.CommonDataKinds.Phone.CONTACT_ID, StringUtil.toString(hasids)); 
			Cursor cursor1 = cr.query(
					ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
					new String[]{ContactsContract.CommonDataKinds.Phone.CONTACT_ID,
							ContactsContract.CommonDataKinds.Phone.TYPE,
							ContactsContract.CommonDataKinds.Phone.NUMBER
							}, whereSelection, null, null);
			if (null != cursor1) {
				if (cursor1.moveToFirst()) {
					do {
						int contactid = cursor1.getInt(0);
						Contact info = mapinfos.get(contactid);
						int type = cursor1.getInt(1);
						if (Phone.TYPE_HOME == type) {
							info.tel = cursor1.getString(2);
						} else if (Phone.TYPE_MOBILE == type) {
							info.mobile = cursor1.getString(2);
						}
					} while (cursor1.moveToNext());
				}
				cursor1.close();
			}
		}
		
		// step3: read email by contactid
		if(!ids.isEmpty()) {
			String whereSelection = String.format( "%s IN (%s)",
					ContactsContract.CommonDataKinds.Phone.CONTACT_ID, StringUtil.toString(hasids));
			Cursor cursor2 = cr.query(Email.CONTENT_URI,
					new String[]{Email.CONTACT_ID, Email.DATA}, whereSelection, null, null);
			if (null != cursor2) {
				if (cursor2.moveToFirst()) {
					do {
						int contactid = cursor2.getInt(0);
						Contact info = mapinfos.get(contactid);
						info.email = cursor2.getString(1);
					} while(cursor2.moveToNext());
				}
				cursor2.close();
			}
		}
		
		ids = null;
		hasids = null;
		mapinfos = null;
		
		return infos;
	}
}