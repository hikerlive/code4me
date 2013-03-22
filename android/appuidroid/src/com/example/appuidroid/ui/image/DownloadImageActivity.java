package com.example.appuidroid.ui.image;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

import com.example.appuidroid.R;

/**
 * 演示说明
 * 	1.演示下载远程图片到本地
 * 
 */
public class DownloadImageActivity extends Activity {
	String imageUrl = "http://www.baidu.com/img/baidu_sylogo1.gif";
	String baikuIconUrl = "http://wap.baiku.cn/icon.action?reqcode=icon&userid=4b96cc0599b1c94ba49e9cc2c8cec944";
	String testbaikuIconUrl = "http://59.151.117.232/icon.action?reqcode=icon&userid=27ec101b4e0fe5a18396d8614ac8de6a";
	private ImageView imageView;
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.download_image);
		
		View v = (View) findViewById(R.id.start_download);
		v.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Bitmap bitmap = downloadPhoto2(testbaikuIconUrl);
				imageView.setImageBitmap(bitmap);
			}
		});
		
		imageView = (ImageView) findViewById(R.id.image_view);
	}
	
	public static Bitmap downloadPhoto(String iconUrl) {
		Bitmap bitmap = null;
		
		HttpGet req = new HttpGet(iconUrl);
		HttpClient httpClient = new DefaultHttpClient();
		try {
			HttpResponse resp = httpClient.execute(req);
			if (resp.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
				HttpEntity httpEntity = resp.getEntity();
				InputStream is = httpEntity.getContent();
				bitmap = BitmapFactory.decodeStream(is);
				is.close();
			}
		} catch (ClientProtocolException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return bitmap;
	}
	
	public static Bitmap downloadPhoto2(String iconUrl) {
		Bitmap bitmap = null;
		
		try {
			URL url = new URL(iconUrl);
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			conn.setDoInput(true);
			conn.connect();
			InputStream is = conn.getInputStream();
			bitmap = BitmapFactory.decodeStream(is);
			is.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return bitmap;
	}
}