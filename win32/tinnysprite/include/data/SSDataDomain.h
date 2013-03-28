#ifndef __SSDATADOMAIN_H__
#define __SSDATADOMAIN_H__

#include <google/protobuf/service.h>
#include "ssdatadomain.pb.h"

namespace SSDataDomain
{

class SSDataDomainSvcSync
{
	/**
	 * 帐号接口
	 */
public:
	// 创建帐号
	virtual CreateAccountResponse* CreateAccount(google::protobuf::RpcController* controller, CreateAccountRequest* request) = 0;
	// 获取帐号信息
	virtual GetAccountResponse* GetAccount(google::protobuf::RpcController* controller, GetAccountRequest* request) =  0;
	// 修改密码
	virtual ChangePasswordResponse* ChangePassword(google::protobuf::RpcController* controller, ChangePasswordRequest* request) = 0;

	/**
	 * 个人设置接口
	 */
public:
	// 持久化个人设置
	virtual SetProfileResponse* SetProfile(google::protobuf::RpcController* controller, SetProfileRequest* request) = 0;
	// 获取个人设置
	virtual GetProfileResponse* GetProfile(google::protobuf::RpcController* controller, GetProfileRequest* request) = 0;

	/**
	 * 关系接口
	 */
public:
	// 关注
	virtual FollowResponse* Follow(google::protobuf::RpcController* controller, FollowRequest* request) = 0;
	// 取消关注
	virtual CancelFollowResponse* CancelFollow(google::protobuf::RpcController* controller, CancelFollowRequest* request) = 0;
	// 获取关注者
	virtual GetFollowingResponse* GetFollowing(google::protobuf::RpcController* controller, GetFollowingRequest* request) = 0;
	// 获取朋友
	//virtual GetFriendResponse* GetFriend(google::protobuf::RpcController* controller, GetFriendRequest* request) = 0;
	// 获取跟随者数量
	virtual GetFollowerCountResponse* GetFollowerCount(google::protobuf::RpcController* controller, GetFollowerCountRequest* request) = 0;
	// 获取跟随者
	virtual GetFollowerResponse* GetFollower(google::protobuf::RpcController* controller, GetFollowerRequest* request) = 0;
	// 保存邀请记录
	virtual InviteResponse* Invite(google::protobuf::RpcController* controller, InviteRequest* request) = 0;
	// 查询邀请记录
	virtual GetInvitingResponse* GetInviting(google::protobuf::RpcController* controller, GetInvitingRequest* request) = 0;
	// 查询被邀请记录
	virtual GetInviterResponse* GetInviter(google::protobuf::RpcController* controller, GetInviterRequest* request) = 0;

	/**
	 * 会话持久接口
	 */
public:
	// 持久会话信息
	virtual SetSessionInfoResponse* SetSessionInfo(google::protobuf::RpcController* controller, SetSessionInfoRequest* request) = 0;
	// 获取会话信息
	virtual GetSessionInfoResponse* GetSessionInfo(google::protobuf::RpcController* controller, GetSessionInfoRequest* request) = 0;

	/**
	 * 消息接口
	 */
public:
	//添加消息
	//virtual AddMessageResponse* AddMessage(google::protobuf::RpcController* controller, AddMessageRequest* request) = 0;
	//删除消息
	//virtual RemoveMessageResponse* RemoveMessage(google::protobuf::RpcController* controller, RemoveMessageRequest* request) = 0;
	//获取消息
	virtual GetMessageResponse* GetMessage(google::protobuf::RpcController* controller, GetMessageRequest* request) = 0;
	// 通过时间或序号获取消息
	virtual GetMessageByTimestampResponse* GetMessageByTimestamp(google::protobuf::RpcController* controller, GetMessageByTimestampRequest* request) = 0;
	// 公开分享，将消息保存到PUBBOX
	virtual SendPublicMessageResponse* SendPublicMessage(google::protobuf::RpcController* controller, SendPublicMessageRequest* request) = 0;
	// 指定分享，将消息保存到发送者的OUTBOX和接收者的INBOX
	virtual SendMessageResponse* SendMessage(google::protobuf::RpcController* controller, SendMessageRequest* request) = 0;
	// 收藏，将消息保存到FAVBOX
	virtual AddFavoriteMessageResponse* AddFavoriteMessage(google::protobuf::RpcController* controller, AddFavoriteMessageRequest* request) = 0;

	/**
	 * 联系人接口
	 */
public:
	// 添加或修改联系人
	virtual AddContactResponse* AddContact(google::protobuf::RpcController* controller, AddContactRequest* request) = 0;
	// 删除联系人，应该先把联系人移出所有的组再调用此方法
	virtual RemoveContactResponse* RemoveContact(google::protobuf::RpcController* controller, RemoveContactRequest* request) = 0;
	// 获取所有联系人
	virtual GetContactResponse* GetContact(google::protobuf::RpcController* controller, GetContactRequest* request) = 0;
	// 添加或修改组
	virtual AddGroupResponse* AddGroup(google::protobuf::RpcController* controller, AddGroupRequest* request) = 0;
	// 删除组，应该先把组里的所有成员移出再调用此方法
	virtual RemoveGroupResponse* RemoveGroup(google::protobuf::RpcController* controller, RemoveGroupRequest* request) = 0;
	// 获取组
	virtual GetGroupResponse* GetGroup(google::protobuf::RpcController* controller, GetGroupRequest* request) = 0;
	// 添加组成员
	virtual AddMemberResponse* AddMember(google::protobuf::RpcController* controller, AddMemberRequest* request) = 0;
	// 删除组成员
	virtual RemoveMemberResponse* RemoveMember(google::protobuf::RpcController* controller, RemoveMemberRequest* request) = 0;
	// 获取组成员
	virtual GetMemberResponse* GetMember(google::protobuf::RpcController* controller, GetMemberRequest* request) = 0;

	/**
	 * 新增接口
	 */
public:
	//若key存在，则返回该key对应的value，并根据isUpdate进行更新
	//若key不存在，则返回新的value，并一定更新
	virtual QueryAndUpdateResponse* QueryAndUpdate(google::protobuf::RpcController* controller, QueryAndUpdateRequest* request) = 0;

	//若key存在，则返回该key对应的value，并删除该条数据
	//若key不存在，则返回null
	virtual QueryAndDeleteResponse* QueryAndDelete(google::protobuf::RpcController* controller, QueryAndDeleteRequest* request) = 0;

	// 添加逻辑节点订阅信息
	virtual AddLogicNodeSubscribeInfoResponse* AddLogicNodeSubscribeInfo(google::protobuf::RpcController* controller, AddLogicNodeSubscribeInfoRequest* request) = 0;
	// 删除逻辑节点订阅信息
	virtual RemoveLogicNodeSubscribeInfoResponse* RemoveLogicNodeSubscribeInfo(google::protobuf::RpcController* controller, RemoveLogicNodeSubscribeInfoRequest* request) = 0;
	// 获取逻辑节点订阅信息
	virtual GetLogicNodeSubscribeInfoResponse* GetLogicNodeSubscribeInfo(google::protobuf::RpcController* controller, GetLogicNodeSubscribeInfoRequest* request) = 0;

	/**
	 * 复合或拆分接口
	 */
public:
	virtual GetFollowingWithProfileResponse* GetFollowingWithProfile(google::protobuf::RpcController* controller, GetFollowingWithProfileRequest* request) = 0;
	virtual GetFollowerWithProfileResponse* GetFollowerWithProfile(google::protobuf::RpcController* controller, GetFollowerWithProfileRequest* request) = 0;
	virtual GetContactExResponse* GetContactEx(google::protobuf::RpcController* controller, GetContactExRequest* request) = 0;
};

}

#endif