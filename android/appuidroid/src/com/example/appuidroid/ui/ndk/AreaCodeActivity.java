package com.example.appuidroid.ui.ndk;

import java.util.ArrayList;

import com.channelsoft.baikudialer.utils.AreaCodeUtil;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;

public class AreaCodeActivity extends Activity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		testCaseAreaCode();
	}
	
	private void testCaseAreaCode() {
		ArrayList<String> numbers = new ArrayList<String>();
		numbers.add("13010421999");
		numbers.add("13010422999");
		numbers.add("13010423999");
		numbers.add("13010424999");
		numbers.add("13010425999");
		numbers.add("13010426999");
		numbers.add("13010427999");
		numbers.add("13010428999");
		numbers.add("13010429999");

		numbers.add("13000000000");
		numbers.add("13000999999");
		numbers.add("13001000000");
		numbers.add("13909081234");
		numbers.add("13909095678");
		numbers.add("13010109012");
		numbers.add("13010119999");
		numbers.add("13010158787");
		numbers.add("13010178888");
		numbers.add("13010189791");
		numbers.add("13430000000");
		numbers.add("13490000000");
		numbers.add("13490999999");
		numbers.add("13489999999");
		numbers.add("13999999999");

		numbers.add("13500920000");
		numbers.add("13500921999");
		numbers.add("13500922011");
		numbers.add("13000199999");
		numbers.add("13909723333");
		numbers.add("13909724999");

		numbers.add("13331107322"); 
		numbers.add("13581877736");
		numbers.add("13581512085");

		numbers.add("15095916313");
		numbers.add("15011136419");
		numbers.add("15305518685");
		numbers.add("15801345017");
		numbers.add("15870780763");
		numbers.add("15855107795");

		numbers.add("18610107039"); /* 联通 */
		numbers.add("18600049977");
		numbers.add("18612706120");

		numbers.add("18089436108");
		numbers.add("18993987281"); /* 电信 */
		numbers.add("18910888200");

		numbers.add("13818437469");
		
		numbers.add("079472651345");
		numbers.add("01088371098");
		numbers.add("055162798602");
		
		AssetManager am = getApplicationContext().getAssets();
		for (int i=0; i<numbers.size(); i++) {
			String areacode = AreaCodeUtil.getAreaCodeText(am, numbers.get(i));
			Log.d("jni", "areacode:" + areacode);
		}		
	}
}