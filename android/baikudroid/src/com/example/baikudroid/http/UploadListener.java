package com.example.baikudroid.http;

public interface UploadListener {
	void onUploadBegin();
	void onUploadProgress(long progress);
	void onUploadFinish();
	void onUploadFailed(int statusCode);
}