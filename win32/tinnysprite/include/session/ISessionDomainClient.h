#ifndef _ISESSIONDOMAINCLIENT_H_
#define _ISESSIONDOMAINCLIENT_H_

#pragma once

#include <string.h>

#define __SD_CLIENT_DLL_LIB_NAME__ "BusinessStub"

#ifdef __SD_CLIENT_DLL_INTERNAL_USE__

#define SD_CLIENT_DLL_API __declspec(dllexport)
#define SD_CLIENT_DLL_CLASS __declspec(dllexport)

#else

#define SD_CLIENT_DLL_API __declspec(dllimport)
#define SD_CLIENT_DLL_CLASS __declspec(dllimport)

#ifndef SD_CLIENT_DLL_NO_LIB
#pragma comment(lib, __SD_CLIENT_DLL_LIB_NAME__".lib")
#pragma message( "自动连接 "__SD_CLIENT_DLL_LIB_NAME__".lib")
#endif

#endif

#pragma pack(push)
#pragma pack(1)

#define SD_CLIENT_NAMESPACE_BEGIN namespace GOUMANG{ namespace ShareSpirit { namespace SessionDomain{
#define SD_CLIENT_NAMESPACE_END };};};


//SD_CLIENT_NAMESPACE_BEGIN;
enum CallBackCMDID
{
	ID_RegistResp		= 1,	//注册成功回调
	ID_LoginResp		= 2,	//登录成功

	ID_ProfileResp,			//用户profile信息回调
	ID_ContactsResp,			//用户联系人回调
	ID_GroupResp,               //用户组信息回调
	ID_GroupMembersResp,

	ID_IconResp,
	
	ID_SetContactResp,
	ID_AddContactsResp,
	ID_RemoveContactsResp,

	ID_SetGroupResp,
	ID_AddGroupResp,
	ID_RemoveGroupResp,

	ID_AddMembersResp,
	ID_RemoveMembersResp,

	ID_FollowCountResp,        //获取多少人关注自己的回调
	ID_FollowListResp,

	ID_FollowingCountResp,		//获取我关注的人数
	ID_FollowingListResp,		//获取我关注的列表

	ID_OtherFollowCountResp,
	ID_OtherFollowListResp,

	ID_OtherFollowingCountResp,
	ID_OtherFollowingListResp,

	ID_InviteResp,
	ID_InviterListMsg,

	ID_GetLastTimeResp,
	ID_SetLastTimeResp,
	ID_OtherUserProfileResp,    //其他用户的profile信息回调
	ID_LastPubShareSeqNumMsg,		//回调上次公开分享的最后序号
	ID_GetPublicShareOutBoxResp,		//回调跟随用户分享过来的消息
    ID_GetProtectShareOutBoxResp,
	ID_GetPrivateBoxResp,
	ID_GetInBoxResp,
	ID_GetOtherPublicShareResp,
	ID_OnShareMsg,                            //其他用户分享过来的消息
    ID_ForgetPwdResp,
	ID_VerifyActiveCodeResp,
	ID_ChangePwdResp,
    ID_UserExistResp,
	ID_FollowUserResp,
    ID_UnFollowUserResp,
	ID_ShareMsgResp,
	ID_OnLogOutMsg,
	ID_InitResult,
	ID_ResetPWD,
	ID_NULL,
};

enum SSResultCode
{
	Result_Success						= 0,
	Result_ParameterWrong				= 30000,	//参数错误，比如传入了一个NULL指针，这个错误一般是立即返回的
	Result_FunctionNoImplement,			//该功能没有实现	
	Result_InternalFail,				//出现内部错误，内部bug导致的

	Result_SendFail,					//发送数据
	Result_SendTimeOut,					//发送数据超时
	Result_ReceiveFail,					//接收数据错误
	Result_ReceiveTimeOut,				//接收数据超时
	Result_ConnectFail,					//连接失败

	Result_InitFail,					//初始化错误
	Result_UserIsRegisted,				//该用户已经注册
	Result_UserNotExist,				//用户不存在
	Result_PasswordError,				//密码错误
	Result_SendEmailError,				//发送Email错误
	Result_VerifyActiveCode,			//校验码错误
	Result_VerifyActiveCodeTimeOut,		//校验码不存在
	Result_HaveFollowed,				//已经跟随了
	Result_HaveNotFollowed,				//还未跟随
    Result_UserNotLogin,                //未登录

	//Result_ResetPasswordError,			//重置密码错误
	//Result_ChangePasswordError,			//修改密码错误
	//Result_HasInit					    = 30000,
	//Result_InitCallBackFunIsNuLL		,
	//Result_HasNotInit					,
	//Result_RegisterNickOrEmailIsNULL	,
	//Result_RegisterActiveCodeIsNULL		,
	//Result_ResetPWVerifyCodeIsNULL		,
	//Result_NetError,//网络异常
	//Result_GetMsgEndSeqNumIsError		,
	//Result_GetMsgCountIsError			,
	//Result_EmailError,//发送Email错误
	//Result_RegistCheckCode,	//校验码错误
	//Result_ConnectServerFail,
	//Regist_OK				= 40000,	//注册OK，请到邮箱去校验
	//Result_Regist_User_Has_Exist	,	//注册已经存在该用户
	//Result_Regist_Send_Email_Error	,	//注册时发送激活邮件失败
	//Regist_Success			,	//注册成功了。通知其登录
	//SS_Login_OK				,
	//Result_SS_Login_User_Not_Exist ,
	//Result_SS_Login_Password_Error	,
	//RegistCheck_SUCCESS		,	//校验成功
	//Result_RegistCheck_Not_Exist	,	//校验信息不存在
	
	//Result_RegistCheck_User_HasRegist	,	//用户已经注册了
	
	//Result_ServerSomeFail,
	
	//Result_Unserialize_Fail,
	//Result_Serialize_Fail,
};

#define USER_ID_LENGTH 32
#define USER_ID_SIZE USER_ID_LENGTH+1
#define MAX_STR_LENGTH 256
#define MAX_ICON_LENGTH 16*1024

struct FollowCountRespT
{
	//char userID;
	char userID[USER_ID_SIZE];
	int count;
};

struct PublicShareLastSequenceNumber
{
//	char				m_UserId;
	int						m_seqNum;
};

struct SSGroupT
{
	char	groupid[USER_ID_SIZE];
	char	name[MAX_STR_LENGTH];
	int	    proprety ; // 是系统组还是用户自定义组,0:系统组,1:自定义组
	//char	extend; // 扩展字段，可以放任何东西
	char *extend;
	unsigned int LengthEx;
    SSGroupT() { memset(this, 0, sizeof(SSGroupT)); proprety = -1;}
    ~SSGroupT() { if (extend != NULL) delete extend; }
};

struct SSContactsT
{
	char	contactid[USER_ID_SIZE]; 
	char	userid[USER_ID_SIZE];	// 系统用户才有
	char	user[MAX_STR_LENGTH];   //email或手机号
	char	nickname[MAX_STR_LENGTH];
	char	nickalias[MAX_STR_LENGTH]; //备注名称
	char	iconname[MAX_STR_LENGTH];
	char	remark[MAX_STR_LENGTH];
	int      age;
	int      sex;
	bool	Issystem;
	int		IsFollowing; //0不是,1表示是跟随者
	int		IsFollower;
	int		IsInvite;
	char* 	extend;
	unsigned int LengthEx;
    SSContactsT() { memset(this, 0, sizeof(SSContactsT)); sex = -1;}
    ~SSContactsT() { if (extend != NULL) delete extend; }
};


struct SSContactsExT
{
	char contactid[USER_ID_SIZE]; // 系统用户才有
	char userid[USER_ID_SIZE];	// 系统用户才有
	char user[MAX_STR_LENGTH];   //email或手机号
	char nickalias[MAX_STR_LENGTH]; //备注名称
    SSContactsExT() { memset(this, 0, sizeof(SSContactsExT));}
    ~SSContactsExT() { }
};

struct SSIconT
{
	char userid[USER_ID_SIZE];
	char iconname[MAX_STR_LENGTH];
	char icon[MAX_ICON_LENGTH];
	unsigned int LengthIcon;
    SSIconT() { memset(this, 0, sizeof(SSIconT));}
    ~SSIconT() { }
};

struct SSProfileT
{
  char		userid[USER_ID_SIZE];
  int		age;
  int		sex;
  char		nickname[MAX_STR_LENGTH];
  char		remark[MAX_STR_LENGTH];			//心情 haoxq change at 2010-10-09
  char		iconname[MAX_STR_LENGTH];
  char		icon[MAX_ICON_LENGTH];
  unsigned int LengthIcon;
  char* 	extend;
  unsigned int LengthEx;
  SSProfileT() { memset(this, 0, sizeof(SSProfileT)); sex = -1;}
  ~SSProfileT() { if (extend != NULL) delete extend; }
};

struct SSLastTimeT
{
	char last_user_time[MAX_STR_LENGTH];
	char last_group_time[MAX_STR_LENGTH];
	char last_contact_time[MAX_STR_LENGTH];
	char last_member_time[MAX_STR_LENGTH];
	char last_follow_time[MAX_STR_LENGTH];
	char last_send_time[MAX_STR_LENGTH];
	char last_recv_time[MAX_STR_LENGTH];
	char last_favo_time[MAX_STR_LENGTH];
    SSLastTimeT() { memset(this, 0, sizeof(SSLastTimeT)); }
    ~SSLastTimeT() { }
};

struct SSUserExistT
{
	char userId[USER_ID_SIZE];
	bool		isSystem;
    SSUserExistT() { memset(this, 0, sizeof(SSUserExistT));}
    ~SSUserExistT() { }
};
//多个groupid
struct SSGroupMembersT
{
	char groupid[USER_ID_SIZE];
	char contactid[USER_ID_SIZE];
    SSGroupMembersT() { memset(this, 0, sizeof(SSGroupMembersT));}
    ~SSGroupMembersT() { }
};

//vector<SSGroupMembersT> 
struct FollowListRespT
{
	char userID[USER_ID_SIZE];  //关注目标的userid
	SSProfileT * userInfo;
	unsigned int Length;
	FollowListRespT() { memset(this, 0, sizeof(FollowListRespT));}
	~FollowListRespT() { if (userInfo != NULL) delete userInfo; }
};

struct LastPubShareSeqNumRespT
{
     int seqNum;
};
// 消息类型
//enum  ShareType{
//	SEND_PUBLIC = 0,	// 公开分享的消息
//	SEND_PROTECT = 1,	// 指定分享发送的消息
//	RECIEVE_FROM = 2,	// 指定分享接收的消息
//	FAVORITE = 3,		//添加到收藏夹的消息
//};
enum  ShareType{
	PUBLIC_SHARE = 1,	// 公开分享的消息
	PROTECT_SHARE = 2,	// 指定分享发送的消息
	PRIVATE_SHARE = 3,	// 添加到收藏夹的消息
//	RECIEVE_SHARE = 4,	//指定分享接收的消息
};
// 内容类型
enum  SSMessageType{	//haoxq change at 2010-10-09
	SM_NORMAL = 0,		// 普通内容
	SM_PROFILE = 100,	// 内容为命令
	SM_ICON = 101,		// 内容为命令
};
#define MSG_ID_LENGTH 48
#define MSG_ID_SIZE MSG_ID_LENGTH+1

struct ShareMsg
{ 
	char					msgid[MSG_ID_SIZE]; // 由8字节的时间或者序号+16字节的uuid
	// 时间或序号序列化8字节时应该采用Big Endian排列.
	//公开分享前面8字节为序号，指定分享为时间
	char					parentMsgid[MSG_ID_SIZE];//转发的时候上一级的msgid
	char					rootMsgid[MSG_ID_SIZE];  //原始发送者的msgid 
	ShareType				type;
	int						seqNum;
	__int64					timestamp; // 此字段可选，也就是msgid前面的8字节
	char					sender[USER_ID_SIZE];
	char*					recievers;					//一个是userid的长度，32*个数
	unsigned int			LengthRecv;					//表示recievers的数据长度，固定为32*个数，不包含结尾0的长度
	char					subject[MAX_STR_LENGTH]; // 消息主题，网页为标题，一句话则全部放在这里
	SSMessageType			Msgtype;
	char					*body;
	unsigned int			LengthBody;
    ShareMsg() { memset(this, 0, sizeof(ShareMsg));}
    ~ShareMsg() { if (body != NULL) delete body; if (recievers != NULL) delete recievers; }
};

typedef void (*OnSessionDomainCallBack)(unsigned int index, CallBackCMDID id, SSResultCode result, void * pData, unsigned int param);

class IUserInterface;

/**
* 描述 初始化
*
* @param callBackFun			[IN] 回调函数指针
*
* @return	    操作状态码
*/

SD_CLIENT_DLL_API SSResultCode SDInit(unsigned int index, OnSessionDomainCallBack callBackFun);
/**
* 描述 销毁
*
*
* @return	    操作状态码
*/
SD_CLIENT_DLL_API SSResultCode SDDestroy();

/**
* 描述 注册，调用该接口完成注册功能，异步，返回值并不代表真正注册成功。
*
* @param userID				[IN] 注册的用户ID
* @param password			[IN] 注册的密码。
* @param pszUser			[IN] 注册的邮箱,或手机号
* @param pszNick			[IN] 注册的昵称
* @param pPrivateInfo		[IN] 注册时保留的私有信息指针，为业务层提供可扩展性
* @param PrivateInfoLen		[IN] 注册时保留的私有信息长度。
* @param pUser			[OUT]返回用户的操作实体。

*
* @return	    操作状态码
*/
SD_CLIENT_DLL_API SSResultCode SDRegister(unsigned int index, const char * userID, 
						const char * pszUser,
						const char * pszPwd,
						const char * psznick,
						const char * pExtendInfo,
						unsigned int ExtendLen);
/**
* 描述 登录，用户启动软件，需要登录才能使用
*
* @param userID			[IN] 登录的用户名
* @param pszPassword	[IN] 登录的密码。
* @param pUser			[OUT]返回用户的操作实
*
* @return	    操作状态码
*/
SD_CLIENT_DLL_API SSResultCode SDLogIn(unsigned int index, const char * userID, 
					 const char * md5Pwd,
					IUserInterface ** pIUser);

/**
* 描述 离线，用户退出登录
*
* @param pUser			[IN]某个用户离线传入其操作实体
 * @return	    操作状态码
*/
SD_CLIENT_DLL_API SSResultCode SDLogOut(unsigned int index, IUserInterface * pIUser);

/**
* 描述 忘记密码 
*
* @param userID			[IN] 用户的用户名
* @param pUser			[OUT]返回用户的操作实
*
* @return	    操作状态码
*/
SD_CLIENT_DLL_API SSResultCode SDForgetPassword(unsigned int index, const char * userID);
SD_CLIENT_DLL_API SSResultCode SDVerifyActiveCode(unsigned int index, const char * userID, const char * pszVerifyCode);
SD_CLIENT_DLL_API SSResultCode SDChangePassword(unsigned int index, const char * userID, const char * newPwd);
/**
* 描述 判断某个用户是否为系统注册用户,通过GetAccount实现。
*
* @param checkUserID	[IN] 判断的用户名,每个id长度固定为MAX_ID_LENGTH，以0结尾
*
* @return	    操作状态码
一个一个返回
*/
SD_CLIENT_DLL_API SSResultCode SDUserIsExist(unsigned int index, const char* userIDs); 

class SD_CLIENT_DLL_CLASS IUserInterface
{
public:
	virtual SSResultCode VerifyActiveCode(unsigned int index, const char * pszVerifyCode) = 0;

	virtual SSResultCode ChangePassword(unsigned int index,const char * newPwd) = 0;

	//单条记录返回
	virtual SSResultCode GetProfile(unsigned int index) = 0;
	
	virtual SSResultCode SetProfile(unsigned int index, const ::SSProfileT& userProfile) = 0;
	//id使用分隔符','分割
	//批量记录返回
	virtual SSResultCode GetOtherProfile(unsigned int index, const char* userIDs) = 0;
	//批量提交，单条多次返回
	virtual SSResultCode GetIcons(unsigned int index, const char* userIDs) = 0;

	virtual SSResultCode SetIcon(unsigned int index, const ::SSIconT& userIcon) = 0;	//haoxq change at 2010-10-09

	virtual SSResultCode GetFollowCount(unsigned int index) = 0;

	virtual SSResultCode GetFollowingCount(unsigned int index) = 0;
	//批量返回
	virtual SSResultCode GetFollowList(unsigned int index, const char * lastID, int getCount) = 0;
	//批量返回
	virtual SSResultCode GetFollowingList(unsigned int index) = 0;


    virtual SSResultCode GetOtherFollowCount(unsigned int index, const char * otherUserID) = 0;

	virtual SSResultCode GetOtherFollowingCount(unsigned int index, const char * otherUserID) = 0;

	//批量返回
	virtual SSResultCode GetOtherFollowList(unsigned int index, const char * lastID, int getCount, const char * userID) = 0;

	//批量返回
	virtual SSResultCode GetOtherFollowingList(unsigned int index, const char * userID) = 0;

	//批量返回
	virtual SSResultCode GetContacts (unsigned int index) = 0;//SSContactsT,批量返回，内部需要结尾

	virtual SSResultCode SetContact(unsigned int index, const ::SSContactsExT & contact) = 0;			//修改联系人信息

	virtual SSResultCode AddContacts(unsigned int index, const ::SSContactsExT* contacts, unsigned int length) = 0; //无论成功与否，每个都返回结果，一个个返回
	//id使用分隔符','分割
	virtual SSResultCode RemoveContacts(unsigned int index, const char* contactIDs) = 0;	//无论成功与否，每个都返回结果，一个个返回

	//批量返回
	virtual SSResultCode GetGroups(unsigned int index) = 0;//批量返回，内部需要结尾

	virtual SSResultCode SetGroup(unsigned int index, const ::SSGroupT &) = 0;

	virtual SSResultCode AddGroup(unsigned int index, const ::SSGroupT &) = 0;
	
	//id使用分隔符','分割
	virtual SSResultCode RemoveGroup(unsigned int index, const char* groupIDs) = 0;

	//单条返回
	virtual SSResultCode GetLastTime(unsigned int index) = 0;

	virtual SSResultCode SetLastTime(unsigned int index, const ::SSLastTimeT& lasttime) = 0; 

	//批量返回
	virtual SSResultCode GetGroupMembers(unsigned int index) = 0;//批量返回，内部需要结尾

	virtual SSResultCode AddGroupMembers(unsigned int index, const ::SSGroupMembersT* members, unsigned int length ) = 0;

	virtual SSResultCode RemoveGroupMembers(unsigned int index, const ::SSGroupMembersT* members, unsigned int length ) = 0;

	virtual SSResultCode PublicShare(unsigned int index, const ::ShareMsg &shareContent) = 0;

	virtual SSResultCode ProtectShare(unsigned int index, const ::ShareMsg &shareContent) = 0;

	virtual SSResultCode PrivateShare(unsigned int index, const ::ShareMsg & shareContent) = 0;

	//单条返回
	virtual SSResultCode GetMsgFromInBox(unsigned int index, const char* msgid, int      getCount) = 0; //每次只返回一条

	//id使用分隔符','分割
	//单条返回
	virtual SSResultCode GetProtectShareMsgFromOutBox(unsigned int index, const char* msgid,
											          int      getCount) = 0;

	//id使用分隔符','分割
	//单条返回
	virtual SSResultCode GetPublicShareMsgFromOutBox(unsigned int index,  const char* msgid,
													  int    getCount) = 0;

	//单条返回
	virtual SSResultCode GetOtherUserPublicShareMsg(unsigned int index, const char* otherUserID,  
													int	   seqNumCursor,
													int    getCount) = 0;
	//响应数据结构
	//userid result,cmd,
	//id使用分隔符','分割
	virtual SSResultCode FollowUser(unsigned int index,  const char* users) = 0;
	//
	virtual SSResultCode UnFollowUser(unsigned int index, const  char* users) = 0;

    // 保存用户的邀请记录
    //     char* users：代表被邀请者的userid列表，用“,”分隔。
    // 回调：
    //     SSResultCode result：Result_Success成功，否则失败
    //     void * pData：等于NULL；
    //     unsigned int param：等于0；
    // 
    // 注：由于被邀请者当前并不是系统用户，所以这里的userid根据email生成，
    // 但不代表系统当前存在这个用户。
    // 
    virtual SSResultCode Invite(unsigned int index, const char* users) = 0;

    // 获取用户的邀请人（谁邀请过我）
    //     
    // 回调：
    //     SSResultCode result：Result_Success成功，否则失败
    //     void * pData：邀请人的SSProfileT数组；
    //     unsigned int param：数组中SSProfileT个数；
    // 
    //virtual SSResultCode GetInviterList(unsigned int index) = 0;
};

#pragma pack(pop)
//SD_CLIENT_NAMESPACE_END;

#endif //_ISESSIONDOMAINCLIENT_H_