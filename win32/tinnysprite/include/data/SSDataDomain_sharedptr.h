#ifndef __SSDATADOMAIN_H__
#define __SSDATADOMAIN_H__

#include <google/protobuf/service.h>
#include "gen-h/ssdatadomain.pb.h"

using namespace google::protobuf;
namespace SSDataDomain
{

class SSDataDomainSvcSync
{
	/**
	 * 帐号接口
	 */
public:
	// 创建帐号
	virtual shared_ptr<CreateAccountResponse> CreateAccount(RpcController* controller, CreateAccountRequest* request) = 0;
	// 获取帐号信息
	virtual shared_ptr<GetAccountResponse> GetAccount(RpcController* controller, GetAccountRequest* request) =  0;
	// 修改密码
	virtual shared_ptr<ChangePasswordResponse> ChangePassword(RpcController* controller, ChangePasswordRequest* request) = 0;

	/**
	 * 个人设置接口
	 */
public:
	// 持久化个人设置
	virtual shared_ptr<SetProfileResponse> SetProfile(RpcController* controller, SetProfileRequest* request) = 0;
	// 获取个人设置
	virtual shared_ptr<GetProfileResponse> GetProfile(RpcController* controller, GetProfileRequest* request) = 0;

	/**
	 * 关系接口
	 */
public:
	// 关注
	virtual shared_ptr<FollowResponse> Follow(RpcController* controller, FollowRequest* request) = 0;
	// 取消关注
	virtual shared_ptr<CancelFollowResponse> CancelFollow(RpcController* controller, CancelFollowRequest* request) = 0;
	// 获取关注者
	virtual shared_ptr<GetFollowingResponse> GetFollowing(RpcController* controller, GetFollowingRequest* request) = 0;
	// 获取朋友
	virtual shared_ptr<GetFriendResponse> GetFriend(RpcController* controller, GetFriendRequest* request) = 0;
	// 获取跟随者数量
	virtual shared_ptr<GetFollowerCountResponse> GetFollowerCount(RpcController* controller, GetFollowerCountRequest* request) = 0;
	// 保存邀请记录
	virtual shared_ptr<InviteResponse> Invite(RpcController* controller, InviteRequest* request) = 0;
	// 查询邀请记录
	virtual shared_ptr<GetInvitingResponse> GetInviting(RpcController* controller, GetInvitingRequest* request) = 0;
	// 查询被邀请记录
	virtual shared_ptr<GetInviterResponse> GetInviter(RpcController* controller, GetInviterRequest* request) = 0;

	/**
	 * 会话持久接口
	 */
public:
	// 持久会话信息
	virtual shared_ptr<SetSessionInfoResponse> SetSessionInfo(RpcController* controller, SetSessionInfoRequest* request) = 0;
	// 获取会话信息
	virtual shared_ptr<GetSessionInfoResponse> GetSessionInfo(RpcController* controller, GetSessionInfoRequest* request) = 0;

	/**
	 * 消息接口
	 */
public:
	//添加消息
	virtual shared_ptr<AddMessageResponse> AddMessage(RpcController* controller, AddMessageRequest* request) = 0;
	//删除消息
	virtual shared_ptr<RemoveMessageResponse> RemoveMessage(RpcController* controller, RemoveMessageRequest* request) = 0;
	//获取消息
	virtual shared_ptr<GetMessageResponse> GetMessage(RpcController* controller, GetMessageRequest* request) = 0;
	// 公开分享，将消息保存为“发送的公开分享消息”
	virtual shared_ptr<SendPublicMessageResponse> SendPublicMessage(RpcController* controller, SendPublicMessageRequest* request) = 0;
	// 指定分享，将消息保存为“发送的指定分享消息”和接收者的“接收的指定分享消息”
	virtual shared_ptr<SendProtectedMessageResponse> SendProtectedMessage(RpcController* controller, SendProtectedMessageRequest* request) = 0;
	// 收藏，将消息保存为“收藏的消息”
	virtual shared_ptr<SendPrivateMessageResponse> SendPrivateMessage(RpcController* controller, SendPrivateMessageRequest* request) = 0;

	/**
	 * 联系人接口
	 */
public:
	// 添加或修改联系人
	virtual shared_ptr<AddContactResponse> AddContact(RpcController* controller, AddContactRequest* request) = 0;
	// 删除联系人
	virtual shared_ptr<RemoveContactResponse> RemoveContact(RpcController* controller, RemoveContactRequest* request) = 0;
	// 获取所有联系人
	virtual shared_ptr<GetContactResponse> GetContact(RpcController* controller, GetContactRequest* request) = 0;
	// 添加组或成员
	virtual shared_ptr<AddGroupResponse> AddGroup(RpcController* controller, AddGroupRequest* request) = 0;
	// 删除组或成员
	virtual shared_ptr<RemoveGroupResponse> RemoveGroup(RpcController* controller, RemoveGroupRequest* request) = 0;
	// 重命名组
	virtual shared_ptr<RenameGroupResponse> RenameGroup(RpcController* controller, RenameGroupRequest* request) = 0;
	// 获取组
	virtual shared_ptr<GetGroupResponse> GetGroup(RpcController* controller, GetGroupRequest* request) = 0;

	/**
	 * 新增接口
	 */
public:
	//若key存在，则返回该key对应的value，并根据isUpdate进行更新
	//若key不存在，则返回新的value，并一定更新
	virtual shared_ptr<QueryAndUpdateResponse> QueryAndUpdate(RpcController* controller, QueryAndUpdateRequest* request) = 0;

	//若key存在，则返回该key对应的value，并删除该条数据
	//若key不存在，则返回null
	virtual shared_ptr<QueryAndDeleteResponse> QueryAndDelete(RpcController* controller, QueryAndDeleteRequest* request) = 0;
};

}

#endif