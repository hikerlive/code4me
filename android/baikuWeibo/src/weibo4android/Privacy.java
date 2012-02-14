package weibo4android;

import java.util.ArrayList;
import java.util.List;

import weibo4android.http.PostParameter;
import weibo4android.http.Response;
import weibo4android.org.json.JSONException;
import weibo4android.org.json.JSONObject;

/**
 * 隐私信息
 * @author Reilost
 *
 */
public class Privacy extends WeiboResponse {
	private static final long serialVersionUID = -8143436310519276882L;
	private Boolean allUserCanComment =null;//是否所有人都可以评论此帐号的微博
	private Boolean allUserCanMessage=null;//是否所有人都可以给此帐号发私信
	private Boolean findByRealname=null;//是否可以用过真实姓名搜索到此帐号
	private Boolean showGeo=null;//是否允许微博保存并显示所处的地理位置信息
	private Boolean showBadge=null;//是否展示勋章
	public Boolean getAllUserCanComment() {
		return allUserCanComment;
	}
	public void setAllUserCanComment(Boolean allUserCanComment) {
		this.allUserCanComment = allUserCanComment;
	}
	public Boolean getAllUserCanMessage() {
		return allUserCanMessage;
	}
	public void setAllUserCanMessage(Boolean allUserCanMessage) {
		this.allUserCanMessage = allUserCanMessage;
	}
	public Boolean getFindByRealname() {
		return findByRealname;
	}
	public void setFindByRealname(Boolean findByRealname) {
		this.findByRealname = findByRealname;
	}
	public Boolean getShowGeo() {
		return showGeo;
	}
	public void setShowGeo(Boolean showGeo) {
		this.showGeo = showGeo;
	}
	public Boolean getShowBadge() {
		return showBadge;
	}
	public void setShowBadge(Boolean showBadge) {
		this.showBadge = showBadge;
	}
	public Privacy(JSONObject json) throws WeiboException {
			try {
				if (json.getString("comment") != null) {
					allUserCanComment = "0".equals(json.getString("comment")) ? true: false;
				}
				if (json.getString("message") != null) {
					allUserCanMessage = "0".equals(json.getString("message")) ? true: false;
				}
				if (json.getString("realname") != null) {
					findByRealname = "0".equals(json.getString("realname")) ? true: false;
				}
				if (json.getString("geo") != null) {
					showGeo = "0".equals(json.getString("geo")) ? true : false;
				}
				if (json.getString("badge") != null) {
					showBadge = "0".equals(json.getString("badge")) ? true: false;
				}
			} catch (JSONException je) {
				throw new WeiboException(je.getMessage() + ":" + json.toString(),
						je);
			}
	}
	public Privacy(Response res) throws WeiboException {
		super(res);
		JSONObject json = res.asJSONObject();
		try {
			if(json.getString("comment")!=null){
				allUserCanComment="0".equals(json.getString("comment"))?true:false;
			}
			if(json.getString("message")!=null){
				allUserCanMessage="0".equals(json.getString("message"))?true:false;
			}
			if(json.getString("realname")!=null){
				findByRealname="0".equals(json.getString("realname"))?true:false;
			}
			if(json.getString("geo")!=null){
				showGeo="0".equals(json.getString("geo"))?true:false;
			}
			if(json.getString("badge")!=null){
				showBadge="0".equals(json.getString("badge"))?true:false;
			}
		} catch (JSONException je) {
			throw new WeiboException(je.getMessage() + ":" + json.toString(),
					je);
		}
	}
	
	public Privacy() {
		super();
	}
	public PostParameter[] getParameters(){
		List<PostParameter> list= new ArrayList<PostParameter>();
		if(allUserCanComment!=null)
			list.add(getParameterValue("comment", allUserCanComment));
		if(allUserCanMessage!=null)
			list.add(getParameterValue("message", allUserCanMessage));
		if(findByRealname!=null)
			list.add(getParameterValue("realname", findByRealname));
		if(showGeo!=null)
			list.add(getParameterValue("geo", showGeo));
		if(showBadge!=null)
			list.add(getParameterValue("badge", showBadge));
		return list.toArray(new PostParameter[list.size()]);
		
	}
	
	private PostParameter getParameterValue(String name,Boolean value){
		return new PostParameter(name, value?"0":"1");
	}
	@Override
	public String toString() {
		return "Privacy [allUserCanComment=" + allUserCanComment
				+ ", allUserCanMessage=" + allUserCanMessage
				+ ", findByRealname=" + findByRealname + ", showGeo=" + showGeo
				+ ", showBadge=" + showBadge + "]";
	}
	
	
	
}
