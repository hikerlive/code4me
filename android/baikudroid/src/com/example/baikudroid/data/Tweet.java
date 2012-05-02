package com.example.baikudroid.data;

import android.os.Parcel;
import android.os.Parcelable;

public class Tweet extends Message implements Parcelable {
	private static final String TAG = "Tweet";
	
	public Tweet() {
	}
	
	public Tweet(Parcel in) {
	}
	
	public static final Parcelable.Creator<Tweet> CREATOR = new Parcelable.Creator<Tweet>() {
		public Tweet createFromParcel(Parcel in) {
			return new Tweet(in);
		}

		public Tweet[] newArray(int size) {
			// return new Tweet[size];
			throw new UnsupportedOperationException();
		}
	};
	
	public int describeContents() {
		return 0;
	}
	
	public void writeToParcel(Parcel out, int flags){
	}
	
	@Override
	public String toString() {
		return "";
	}
}