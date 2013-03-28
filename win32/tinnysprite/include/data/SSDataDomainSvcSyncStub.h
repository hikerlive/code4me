#ifndef __SSDATADOMAINSVCSYNCSTUB_H__
#define __SSDATADOMAINSVCSYNCSTUB_H__

#include "SSDataDomain.h"
//#include "SSObjectPool.h"

namespace SSDataDomain {

class SSDataDomainSvcSyncStub : public SSDataDomainSvcSync
{
private:
	SSDataDomainSvc::Stub stub;
	int timeout;

	//static HANDLE event_create_func();
	//static void event_destroy_func(HANDLE evt);
	//SSObjectPool<HANDLE> event_pool;

public:
	SSDataDomainSvcSyncStub(google::protobuf::RpcChannel* channel);
	~SSDataDomainSvcSyncStub();
	void SetTimeout(int ms) { this->timeout = ms; }
	int GetTimeout() { return this->timeout; }

private:
	static void CallCompletion(HANDLE evt);

	template<typename TRequest, typename TResponse>
	TResponse* CallMethod(
		void (SSDataDomainSvc::Stub::*method)(google::protobuf::RpcController*, const TRequest*, TResponse*, google::protobuf::Closure*),
		google::protobuf::RpcController* controller,
		TRequest* request);

	/**
	 * 帐号接口
	 */
public:
	// 创建帐号
	CreateAccountResponse* CreateAccount(google::protobuf::RpcController* controller, CreateAccountRequest* request);
	// 获取帐号信息
	GetAccountResponse* GetAccount(google::protobuf::RpcController* controller, GetAccountRequest* request);
	// 修改密码
	ChangePasswordResponse* ChangePassword(google::protobuf::RpcController* controller, ChangePasswordRequest* request);

	/**
	 * 个人设置接口
	 */
public:
	// 持久化个人设置
	SetProfileResponse* SetProfile(google::protobuf::RpcController* controller, SetProfileRequest* request);
	// 获取个人设置
	GetProfileResponse* GetProfile(google::protobuf::RpcController* controller, GetProfileRequest* request);

	/**
	 * 关系接口
	 */
public:
	// 关注
	FollowResponse* Follow(google::protobuf::RpcController* controller, FollowRequest* request);
	// 取消关注
	CancelFollowResponse* CancelFollow(google::protobuf::RpcController* controller, CancelFollowRequest* request);
	// 获取关注者
	GetFollowingResponse* GetFollowing(google::protobuf::RpcController* controller, GetFollowingRequest* request);
	// 获取朋友
	//GetFriendResponse* GetFriend(google::protobuf::RpcController* controller, GetFriendRequest* request);
	// 获取跟随者数量
	GetFollowerCountResponse* GetFollowerCount(google::protobuf::RpcController* controller, GetFollowerCountRequest* request);
	// 获取跟随者
	GetFollowerResponse* GetFollower(google::protobuf::RpcController* controller, GetFollowerRequest* request);
	// 保存邀请记录
	InviteResponse* Invite(google::protobuf::RpcController* controller, InviteRequest* request);
	// 查询邀请记录
	GetInvitingResponse* GetInviting(google::protobuf::RpcController* controller, GetInvitingRequest* request);
	// 查询被邀请记录
	GetInviterResponse* GetInviter(google::protobuf::RpcController* controller, GetInviterRequest* request);

	/**
	 * 会话持久接口
	 */
public:
	// 持久会话信息
	SetSessionInfoResponse* SetSessionInfo(google::protobuf::RpcController* controller, SetSessionInfoRequest* request);
	// 获取会话信息
	GetSessionInfoResponse* GetSessionInfo(google::protobuf::RpcController* controller, GetSessionInfoRequest* request);

	/**
	 * 消息接口
	 */
public:
	//添加消息
	//AddMessageResponse* AddMessage(google::protobuf::RpcController* controller, AddMessageRequest* request);
	//删除消息
	//RemoveMessageResponse* RemoveMessage(google::protobuf::RpcController* controller, RemoveMessageRequest* request);
	//获取消息
	GetMessageResponse* GetMessage(google::protobuf::RpcController* controller, GetMessageRequest* request);
	// 通过时间或序号获取消息
	GetMessageByTimestampResponse* GetMessageByTimestamp(google::protobuf::RpcController* controller, GetMessageByTimestampRequest* request);
	// 公开分享，将消息保存到PUBBOX
	SendPublicMessageResponse* SendPublicMessage(google::protobuf::RpcController* controller, SendPublicMessageRequest* request);
	// 指定分享，将消息保存到发送者的OUTBOX和接收者的INBOX
	SendMessageResponse* SendMessage(google::protobuf::RpcController* controller, SendMessageRequest* request);
	// 收藏，将消息保存到FAVBOX
	AddFavoriteMessageResponse* AddFavoriteMessage(google::protobuf::RpcController* controller, AddFavoriteMessageRequest* request);

	/**
	 * 联系人接口
	 */
public:
	// 添加或修改联系人
	AddContactResponse* AddContact(google::protobuf::RpcController* controller, AddContactRequest* request);
	// 删除联系人
	RemoveContactResponse* RemoveContact(google::protobuf::RpcController* controller, RemoveContactRequest* request);
	// 获取所有联系人
	GetContactResponse* GetContact(google::protobuf::RpcController* controller, GetContactRequest* request);
	// 添加组或成员
	AddGroupResponse* AddGroup(google::protobuf::RpcController* controller, AddGroupRequest* request);
	// 删除组或成员
	RemoveGroupResponse* RemoveGroup(google::protobuf::RpcController* controller, RemoveGroupRequest* request);
	// 获取组
	GetGroupResponse* GetGroup(google::protobuf::RpcController* controller, GetGroupRequest* request);
	// 添加组成员
	AddMemberResponse* AddMember(google::protobuf::RpcController* controller, AddMemberRequest* request);
	// 删除组成员
	RemoveMemberResponse* RemoveMember(google::protobuf::RpcController* controller, RemoveMemberRequest* request);
	// 获取组成员
	GetMemberResponse* GetMember(google::protobuf::RpcController* controller, GetMemberRequest* request);

	/**
	 * 新增接口
	 */
public:
	//若key存在，则返回该key对应的value，并根据isUpdate进行更新
	//若key不存在，则返回新的value，并一定更新
	QueryAndUpdateResponse* QueryAndUpdate(google::protobuf::RpcController* controller, QueryAndUpdateRequest* request);

	//若key存在，则返回该key对应的value，并删除该条数据
	//若key不存在，则返回null
	QueryAndDeleteResponse* QueryAndDelete(google::protobuf::RpcController* controller, QueryAndDeleteRequest* request);

	// 添加逻辑节点订阅信息
	AddLogicNodeSubscribeInfoResponse* AddLogicNodeSubscribeInfo(google::protobuf::RpcController* controller, AddLogicNodeSubscribeInfoRequest* request);
	// 删除逻辑节点订阅信息
	RemoveLogicNodeSubscribeInfoResponse* RemoveLogicNodeSubscribeInfo(google::protobuf::RpcController* controller, RemoveLogicNodeSubscribeInfoRequest* request);
	// 获取逻辑节点订阅信息
	GetLogicNodeSubscribeInfoResponse* GetLogicNodeSubscribeInfo(google::protobuf::RpcController* controller, GetLogicNodeSubscribeInfoRequest* request);

	/**
	 * 复合或拆分接口
	 */
public:
	GetFollowingWithProfileResponse* GetFollowingWithProfile(google::protobuf::RpcController* controller, GetFollowingWithProfileRequest* request);
	GetFollowerWithProfileResponse* GetFollowerWithProfile(google::protobuf::RpcController* controller, GetFollowerWithProfileRequest* request);
	GetContactExResponse* GetContactEx(google::protobuf::RpcController* controller, GetContactExRequest* request);
};

} // namespace

#endif