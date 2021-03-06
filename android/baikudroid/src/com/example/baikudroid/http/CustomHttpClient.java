package com.example.baikudroid.http;


import java.io.File;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.HttpVersion;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.conn.ClientConnectionManager;
import org.apache.http.conn.params.ConnManagerParams;
import org.apache.http.conn.scheme.PlainSocketFactory;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.conn.ssl.SSLSocketFactory;
import org.apache.http.entity.mime.MultipartEntity;
import org.apache.http.entity.mime.content.FileBody;
import org.apache.http.entity.mime.content.StringBody;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.params.HttpProtocolParams;
import org.apache.http.protocol.HTTP;
import org.apache.http.util.EntityUtils;

import android.util.Log;

public class CustomHttpClient {
	private static final String CHARSET = HTTP.UTF_8;
	private static final String TAG = "CustomHttpClient";
	private static HttpClient customerHttpClient;
	private CustomHttpClient() {
	}
	public static synchronized HttpClient getHttpClient() {
		if (null == customerHttpClient) {
			HttpParams params = new BasicHttpParams();
			HttpProtocolParams.setVersion(params, HttpVersion.HTTP_1_1);
			HttpProtocolParams.setContentCharset(params, CHARSET);
			HttpProtocolParams.setUseExpectContinue(params, true);
			HttpProtocolParams.setUserAgent(params, 
					 "Mozilla/5.0(Linux;U;Android 2.2.1;en-us;Nexus One Build.FRG83) "
                    + "AppleWebKit/553.1(KHTML,like Gecko) Version/4.0 Mobile Safari/533.1");
			
			// 从连接池中取连续的超时时间
			ConnManagerParams.setTimeout(params, 1000);
			HttpConnectionParams.setConnectionTimeout(params, 2000);
			HttpConnectionParams.setSoTimeout(params, 4000);
			
			// 设置我们的HttpClient支持HTTP和HTTPS两种模式
			SchemeRegistry schReg = new SchemeRegistry();
			schReg.register(new Scheme("http", 
					PlainSocketFactory.getSocketFactory(),
					80));
			schReg.register(new Scheme("https", 
					SSLSocketFactory.getSocketFactory(),
					443));
			
			// 使用线程安全的连接管理来创建HttpClient
			ClientConnectionManager conMgr = new ThreadSafeClientConnManager(
					params, schReg);
			customerHttpClient = new DefaultHttpClient(conMgr, params);
		}
		return customerHttpClient;
	}
	
	public static String post(String url, NameValuePair... params) {
		try {
			// 编码参数
			List<NameValuePair> formparams = new ArrayList<NameValuePair>();
			for (NameValuePair p : params) {
				formparams.add(p);
			}
			UrlEncodedFormEntity entity = new UrlEncodedFormEntity(formparams, CHARSET);
			
			// 创建POST请求
			HttpPost request = new HttpPost(url);
			request.setEntity(entity);
			
			// 发送请求
			HttpClient client = getHttpClient();
			HttpResponse response = client.execute(request);
			if (response.getStatusLine().getStatusCode() != HttpStatus.SC_OK) {
				throw new RuntimeException("请求参数");
			}
			HttpEntity resEntity = response.getEntity();
			return (resEntity == null) ? null : EntityUtils.toString(resEntity, CHARSET);
		} catch (UnsupportedEncodingException e) {
			Log.w(TAG, e.getMessage());
			return null;
		} catch (ClientProtocolException e) {
			Log.w(TAG, e.getMessage());
			return null;
		} catch (IOException e) {
			throw new RuntimeException("连接失败", e);
		}
	}
	
	public static void post(String url, String filepath, String desp) throws UnsupportedEncodingException {
   		HttpClient httpclient = new DefaultHttpClient();
   		
		HttpPost httppost = new HttpPost(url);  
        FileBody file = new FileBody(new File(filepath));  
        StringBody descript = new StringBody(desp);  
  
        MultipartEntity reqEntity = new MultipartEntity();  
        reqEntity.addPart("file", file);  
        reqEntity.addPart("path", descript); 
        
        httppost.setEntity(reqEntity);
        try {
            HttpResponse response = httpclient.execute(httppost);   
            if(HttpStatus.SC_OK==response.getStatusLine().getStatusCode()){  
                HttpEntity entity = response.getEntity();  
                if (entity != null) {  
                    System.out.println(EntityUtils.toString(entity));  
                }  
                if (entity != null) {  
                    entity.consumeContent();  
                }  
            }
        } catch(Exception e) {
        	e.printStackTrace();
        	Log.e(TAG, "exception", e.getCause());
        }		
	}
	
	public static void post(String url, String filepath, String desp, UploadListener listner) throws UnsupportedEncodingException {
		listner.onUploadBegin();
		HttpClient httpclient = new DefaultHttpClient();
   		
		HttpPost httppost = new HttpPost(url);  
        FileBody file = new FileBody(new File(filepath));  
        StringBody descript = new StringBody(desp);  
  
        MultipartEntity reqEntity = new CountMultipartEntity(listner);  
        reqEntity.addPart("file", file);  
        reqEntity.addPart("path", descript); 
        httppost.setEntity(reqEntity);
        try {
            HttpResponse response = httpclient.execute(httppost);   
            if(HttpStatus.SC_OK==response.getStatusLine().getStatusCode()){  
                HttpEntity entity = response.getEntity();  
                if (entity != null) {  
                    System.out.println(EntityUtils.toString(entity));  
                }  
                if (entity != null) {  
                    entity.consumeContent();  
                } 
            }
        } catch(Exception e) {
        	listner.onUploadFinish();
        	e.printStackTrace();
        	Log.e(TAG, "exception", e.getCause());
        }
        listner.onUploadFinish();
	}

	public static void post(String url, String username, String pwd, String subject, 
			String filepath, String fileType, UploadListener listner) throws UnsupportedEncodingException {
		listner.onUploadBegin();
		HttpClient httpclient = new DefaultHttpClient();
   		
		HttpPost httppost = new HttpPost(url);  
        FileBody file = new FileBody(new File(filepath));  
        StringBody _username = new StringBody(username);
        StringBody _pwd = new StringBody(pwd);
        StringBody _subject = new StringBody(new String(subject.getBytes("iso-8859-1"),"utf-8"));
        StringBody _fileType = new StringBody(fileType);
  
        MultipartEntity reqEntity = new CountMultipartEntity(listner);  
        reqEntity.addPart("file", file);  
        reqEntity.addPart("username", _username);
        reqEntity.addPart("password", _pwd);
        reqEntity.addPart("subject", _subject);
        reqEntity.addPart("type", _fileType);
        httppost.setEntity(reqEntity);
        try {
            HttpResponse response = httpclient.execute(httppost);
            int statusCode = response.getStatusLine().getStatusCode();
            if(HttpStatus.SC_OK==statusCode){  
                HttpEntity entity = response.getEntity();  
                if (entity != null) {  
                    System.out.println(EntityUtils.toString(entity));  
                }  
                if (entity != null) {  
                    entity.consumeContent();  
                } 
            } else {
            	listner.onUploadFailed(statusCode);
            }
        } catch(Exception e) {
        	listner.onUploadFinish();
        	e.printStackTrace();
        	Log.e(TAG, "exception", e.getCause());
        }
        listner.onUploadFinish();
	}
	
}	
