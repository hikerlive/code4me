package com.example.baikudroid.ui.base;

import com.example.baikudroid.R;
import com.example.baikudroid.data.Tweet;
import com.example.baikudroid.ui.module.TweetAdapter;
import com.example.baikudroid.ui.module.TweetCursorAdapter;
import com.example.baikudroid.widget.PullToRefreshListView;
import com.example.baikudroid.widget.PullToRefreshListView.OnRefreshListener;

import android.app.Activity;
import android.view.View;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.os.Bundle;
import android.util.Log;
import android.database.Cursor;

public abstract class BaikuCursorBaseActivity extends BaikuListBaseActivity {
	private static final String TAG = "BaikuCursorBaseActivity";
	
	// Views
	protected PullToRefreshListView mTweetList;
	protected TweetCursorAdapter mTweetAdapter;
	
	protected View mListHeader;
	protected View mListFooter;
	
	protected TextView mLoadMoreBtn;
	protected ProgressBar mLoadMoreGIF;
	protected TextView mLoadMoreBtnTop;
	protected ProgressBar mLoadMoreGIFTOP;
	
	abstract protected Cursor fetchMessages();
	
	@Override
	protected boolean _onCreate(Bundle savedInstanceState) {
		Log.d(TAG, "_onCreate");
		if (super._onCreate(savedInstanceState)) {
			return true;
		}
		return false;
	}
	
	@Override
	protected int getLayoutId() {
		return R.layout.baiku;
	}
	
	@Override
	protected ListView getTweetList(){
		return mTweetList;
	}
	
	@Override
	protected TweetAdapter getTweetAdapter(){
		return mTweetAdapter;
	}
	
	@Override
	protected void setupState() {
		Log.d(TAG, "setupState");
	 	
		mTweetList = (PullToRefreshListView)findViewById(R.id.tweet_list);
		
		// NOTE: 
		mTweetList.setOnRefreshListener(new OnRefreshListener(){
			@Override
			public void onRefresh() {
				// doRetrive() 
			}
		});
		mListFooter = View.inflate(this, R.layout.listview_footer, null);
		mTweetList.addFooterView(mListFooter, null, true);
		
		initDatabase();
		
		// TODO:
		Cursor cursor;
		cursor = fetchMessages();
		mTweetAdapter = new TweetCursorAdapter(this, cursor);
		mTweetList.setAdapter(mTweetAdapter);
	}
	
	@Override
	protected void updateTweet(Tweet tweet){
		
	}
	
    protected void initDatabase() {
    	Log.i(TAG, "initDatabase()");
		Tweet tweet = new Tweet();
		tweet.id = "01";
		tweet.msgid = "000001335869DA20b44437b5a8f783af287025a753c3dae5";
		tweet.shareType = "1";
		tweet.sender = "5172caf958403e7ca854f0d360486fce";
		tweet.screenName = "Hikerlive";
		tweet.iconUrl = "default.png";
		tweet.contents = "指用人单位直接从学校选拔或从其它途径招聘应届毕业生。一般在每年9-11月和次年的3-4月集中进行。校园招聘作为解决应届学生就业问题的最好机会需要各方认真应对。";
		tweet.isFavorited = "false";
		getDb().insertTweet(tweet);
		
		tweet.id = "02";
		tweet.msgid = "000001335869DA205172caf958403e7ca854f0d360486fce";
		tweet.screenName = "和菜头";
		tweet.contents= "休·杰克曼（英文：Hugh Jackman，全名：Hugh Michael Jackman，1968年10月12日）是澳大利亚演员，在电影《范海辛》饰演一脸胡渣粗犷扮相的吸血鬼猎人受到了大众的赏识，更因饰演《X战警》系列中的‘金钢狼’而闻名。其最新主演电影《铁甲钢拳》将于11月8日在中国大陆上映。";
		getDb().insertTweet(tweet);
		
		tweet.id = "03";
		tweet.msgid = "000001335869DA305172caf958403e7ca854f0d360486fce";
		tweet.screenName = "令狐冲";
		tweet.contents= "他可以热烈如火，也可以冷酷似冰，他有温柔的笑容，也有钢一样坚硬的眼神，这个充满着矛盾的神秘男人，戏路甚广又甚少传出绯闻，形象讨好。业余时间，杰克曼会弹弹钢琴和吉他，打打高尔夫或者去风帆冲浪，他最喜欢的食物是寿司。";
		getDb().insertTweet(tweet);
		
		tweet.id = "04";
		tweet.msgid = "000001335869DA405172caf958403e7ca854f0d360486fce";
		tweet.screenName = "金刚狼";
		tweet.contents= "北京时间2011年9月14日00:52 Hugh Jackman 在新浪微博发出了他的第一条微博";
		getDb().insertTweet(tweet);
		
		tweet.id = "05";
		tweet.msgid = "000001335869DA505172caf958403e7ca854f0d360486fce";
		tweet.screenName = "李开复";
		tweet.contents= "1.杰克曼是个在美国出名的澳大利亚人，但他心中最想工作的地方却是在英国，因此他平日里的穿着也都是很英式。2.杰克曼亦是一个皮夹克不离身的人，皮衣里面搭配针织简单有型，平添几分男子气概。";
		getDb().insertTweet(tweet);
		
		tweet.id = "06";
		tweet.msgid = "000001335869DA605172caf958403e7ca854f0d360486fce";
		tweet.screenName = "东邪西毒";
		tweet.contents= "3.硬汉当然少不了皮鞋，或许这种僧侣扣皮鞋比之系带皮鞋更能体现出男人味，尤其是皮鞋边上的金属扣，是很出彩的细节。其它配件如吊牌项链，则是带有军旅风格的时尚品，也很适合这次的搭配主题。";
		getDb().insertTweet(tweet);
    }
}
