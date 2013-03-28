/*--------------------------------------------------------------------------------*\

     文件：  ISessionDomainClient.cpp
     作者：  姚松文
     日期：  2010年10月20日
     说明：  会话域的桩，以替换原有会话域。

     1、运行在客户端进程内；
     2、直接与数据域通信；
     3、消息通过“pull”方式获取

\*--------------------------------------------------------------------------------*/
#include "stdafx.h"

#define __SD_CLIENT_DLL_INTERNAL_USE__
#include <ISessionDomainClient.h>

#include <string>
#include <hash_map>
#include <queue>
#include <memory>

#include "SSDataDomain.api.h"
using namespace SSDataDomainClient;

#include "auto_dispose_ptr.h"
#include "AsyncCallExecutor.h"
#include "MessagePumpStation.h"
#include "DataConverter.h"

///////////////////////////////////////////////////////////////////////////////
// UserInterface的定义
class UserInterface : public IUserInterface {
public:
    std::string m_strUserId;
    bool m_bLogin;

public:
    SSResultCode VerifyActiveCode(unsigned int index, const char * pszVerifyCode);
    SSResultCode ChangePassword(unsigned int index,const char * newPwd);
    SSResultCode GetProfile(unsigned int index);
    SSResultCode SetProfile(unsigned int index, const ::SSProfileT& userProfile);
    SSResultCode GetOtherProfile(unsigned int index, const char* userIDs);
    SSResultCode GetIcons(unsigned int index, const char* userIDs);
    SSResultCode SetIcon(unsigned int index, const ::SSIconT& userIcon);
    SSResultCode GetFollowCount(unsigned int index);
    SSResultCode GetFollowingCount(unsigned int index);
    SSResultCode GetFollowList(unsigned int index, const char * lastID, int getCount);
    SSResultCode GetFollowingList(unsigned int index);
    SSResultCode GetOtherFollowCount(unsigned int index, const char * otherUserID);
    SSResultCode GetOtherFollowingCount(unsigned int index, const char * otherUserID);
    SSResultCode GetOtherFollowList(unsigned int index, const char * lastID, int getCount, const char * userID);
    SSResultCode GetOtherFollowingList(unsigned int index, const char * userID);
    SSResultCode GetContacts (unsigned int index);
    SSResultCode SetContact(unsigned int index, const ::SSContactsExT & contact);
    SSResultCode AddContacts(unsigned int index, const ::SSContactsExT* contacts, unsigned int length);
    SSResultCode RemoveContacts(unsigned int index, const char* contactIDs);
    SSResultCode GetGroups(unsigned int index);
    SSResultCode SetGroup(unsigned int index, const ::SSGroupT &);
    SSResultCode AddGroup(unsigned int index, const ::SSGroupT &);
    SSResultCode RemoveGroup(unsigned int index, const char* groupIDs);
    SSResultCode GetLastTime(unsigned int index);
    SSResultCode SetLastTime(unsigned int index, const ::SSLastTimeT& lasttime);
    SSResultCode GetGroupMembers(unsigned int index);
    SSResultCode AddGroupMembers(unsigned int index, const ::SSGroupMembersT* members, unsigned int length );
    SSResultCode RemoveGroupMembers(unsigned int index, const ::SSGroupMembersT* members, unsigned int length );
    SSResultCode PublicShare(unsigned int index, const ::ShareMsg &shareContent);
    SSResultCode ProtectShare(unsigned int index, const ::ShareMsg &shareContent);
    SSResultCode PrivateShare(unsigned int index, const ::ShareMsg & shareContent);
    SSResultCode GetMsgFromInBox(unsigned int index, const char* msgid, int getCount);
    SSResultCode GetProtectShareMsgFromOutBox(unsigned int index, const char* msgid, int getCount);
    SSResultCode GetPublicShareMsgFromOutBox(unsigned int index,  const char* msgid, int getCount);
    SSResultCode GetOtherUserPublicShareMsg(unsigned int index, const char* otherUserID, int seqNumCursor, int getCount);
    SSResultCode FollowUser(unsigned int index, const char* users);
    SSResultCode UnFollowUser(unsigned int index, const char* users);
    SSResultCode Invite(unsigned int index, const char* users);
    SSResultCode GetInviterList(unsigned int index);
};
///////////////////////////////////////////////////////////////////////////////
// 全局变量
#define ENSURE_INITLIALIZATION() \
    do { \
        if (g_pCallback == NULL) { \
            SSLOG_ERROR(_T("it's not initialized.")); \
            return Result_InitFail; \
        } \
    } while(0)
#define ENSURE_LOGIN() \
    do { \
        if (!m_bLogin) { \
            SSLOG_ERROR(_T("it's not login.")); \
            return Result_UserNotLogin; \
        } \
    } while(0)
#define AFX_MANAGE_STATE(x)

DEFINE_LOGGER(_T("stub.interface"));

static std::string g_login_user;
static IUserInterface* g_pUserInterface = NULL;
static OnSessionDomainCallBack g_pCallback = NULL;
static SSDataDomainSvc* g_pDataDomainSvc = NULL;
static int g_last_seq_number = 0;

void CallbackLog(unsigned int index, CallBackCMDID id, SSResultCode result, void * pData, unsigned int count)
{
#ifndef DISABLE_LOGGER
    if (logger->isDebugEnabled()) {
        ::log4cxx::helpers::TMessageBuffer mb;
        LogStream& oss = mb;
        oss << _T("callback(") << index << _T(",");
        log(oss, id) << _T(",");
        log(oss, result) << _T(",");
        if (pData == NULL) {
            oss << _T("NULL") << _T(",");
        } else {
            TCHAR szBuffer[32];
            memset(szBuffer, 0, sizeof(TCHAR)*32);
            _stprintf(szBuffer, _T("%08X"), pData);
            oss << szBuffer << _T(",");
        }
        oss << count << _T(")");
        logger->forcedLog(::log4cxx::Level::getDebug(), mb.str(oss), LOG4CXX_LOCATION);
    }
    
    if (pData != NULL) {
        if (logger->isDebugEnabled()) {
            ::log4cxx::helpers::TMessageBuffer mb;
            LogStream& oss = mb;
            oss << _T("data: ");
            switch (id) {
            case ID_RegistResp:
                break;
            case ID_LoginResp:
                break;
            case ID_ProfileResp:
                log(oss, (SSProfileT*)pData, count);
                break;
            case ID_ContactsResp:
                log(oss, (SSContactsT*)pData, count);
                break;
            case ID_GroupResp:
                log(oss, (SSGroupT*)pData, count);
                break;
            case ID_GroupMembersResp:
                log(oss, (SSGroupMembersT*)pData, count);
                break;
            case ID_IconResp:
                log(oss, (SSIconT*)pData, count);
                break;
            case ID_SetContactResp:
                break;
            case ID_AddContactsResp:
                break;
            case ID_RemoveContactsResp:
                break;
            case ID_SetGroupResp:
                break;
            case ID_AddGroupResp:
                break;
            case ID_RemoveGroupResp:
                break;
            case ID_AddMembersResp:
                break;
            case ID_RemoveMembersResp:
                break;
            case ID_FollowCountResp:
                oss << *(int*)pData;
                break;
            case ID_FollowListResp:
                log(oss, (SSProfileT*)pData, count);
                break;
            case ID_FollowingCountResp:
                oss << *(int*)pData;
                break;
            case ID_FollowingListResp:
                log(oss, (SSProfileT*)pData, count);
                break;
            case ID_OtherFollowCountResp:
                oss << *(int*)pData;
                break;
            case ID_OtherFollowListResp:
                log(oss, (SSProfileT*)pData, count);
                break;
            case ID_OtherFollowingCountResp:
                oss << *(int*)pData;
                break;
            case ID_OtherFollowingListResp:
                log(oss, (SSProfileT*)pData, count);
                break;
            case ID_GetLastTimeResp:
                log(oss, (SSLastTimeT*)pData, count);
                break;
            case ID_SetLastTimeResp:
                break;
            case ID_OtherUserProfileResp:
                log(oss, (SSProfileT*)pData, count);
                break;
            case ID_LastPubShareSeqNumMsg:
                oss << *(int*)pData;
                break;
            case ID_GetPublicShareOutBoxResp:
                log(oss, (ShareMsg*)pData, count);
                break;
            case ID_GetProtectShareOutBoxResp:
                log(oss, (ShareMsg*)pData, count);
                break;
            case ID_GetPrivateBoxResp:
                log(oss, (ShareMsg*)pData, count);
                break;
            case ID_GetInBoxResp:
                log(oss, (ShareMsg*)pData, count);
                break;
            case ID_GetOtherPublicShareResp:
                log(oss, (ShareMsg*)pData, count);
                break;
            case ID_OnShareMsg:
                log(oss, (ShareMsg*)pData, count);
                break;
            case ID_ForgetPwdResp:
                break;
            case ID_VerifyActiveCodeResp:
                break;
            case ID_ChangePwdResp:
                break;
            case ID_UserExistResp:
                log(oss, (SSUserExistT*)pData, count);
                break;
            case ID_FollowUserResp:
                break;
            case ID_UnFollowUserResp:
                break;
            case ID_ShareMsgResp:
                oss << (char*)pData;
                break;
            case ID_OnLogOutMsg:
                break;
            case ID_InitResult:
                break;
            case ID_ResetPWD:
                break;
            case ID_NULL:
                break;
            }
            logger->forcedLog(::log4cxx::Level::getDebug(), mb.str(oss), LOG4CXX_LOCATION);
        }
    }
#endif //DISABLE_LOGGER
    if (g_pCallback == NULL) {
        SSLOG_DEBUG(_T("g_pCallback == NULL"));
        return;
    }
    g_pCallback(index, id, result, pData, count);
}

///////////////////////////////////////////////////////////////////////////////
// 全局函数
SSResultCode SDInit(unsigned int index, OnSessionDomainCallBack callBackFun) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SDInit(") << index << _T(",") << callBackFun << _T(")"));

    //=========================================================================
    // 初始化DataDomainClient
    Initialize();

    g_pDataDomainSvc = SSDataDomainSvc::New();
    g_pDataDomainSvc->AddEndpoint("59.151.117.231", 29161);
    //g_pDataDomainSvc->AddEndpoint("59.151.117.232", 29161);
    //g_pDataDomainSvc->AddEndpoint("59.151.117.233", 29161);
    //g_pDataDomainSvc->AddEndpoint("127.0.0.101", 9161);
    g_pDataDomainSvc->SetTimeout(10000);
    
    //=========================================================================
    g_pCallback = callBackFun;
    g_async_call_executor->start();
    
    //=========================================================================
    BEGIN_ASYNC_CALL1(unsigned int, index)
    void ctor() {
    }
    void dtor() {
    }
    void call() {
        CallbackLog(index, ID_InitResult, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL1(index)
    //=========================================================================

    return Result_Success;
}

SSResultCode SDDestroy() {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SDDestroy()"));

    g_pCallback = NULL;
    g_async_call_executor->stop();
    
    if (g_pDataDomainSvc != NULL) {
        g_pDataDomainSvc->Dispose();
        g_pDataDomainSvc = NULL;
    }

    g_login_user.clear();
    if (g_pUserInterface != NULL) {
        delete g_pUserInterface;
        g_pUserInterface = NULL;
    }

    // 退出后停止消息泵
    g_message_pump_station->stop();

    Uninitialize();
    return Result_Success;
}

SSResultCode SDRegister(unsigned int index, const char * userID, 
                        const char * pszUser,
                        const char * pszPwd,
                        const char * psznick,
                        const char * pExtendInfo,
                        unsigned int ExtendLen) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SDRegister(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();

    //=========================================================================
    BEGIN_ASYNC_CALL7(unsigned int, index,
                      const char *, userID,
                      const char *, pszUser,
                      const char *, pszPwd,
                      const char *, psznick,
                      const char *, pExtendInfo,
                      unsigned int, ExtendLen)
    void ctor() {
        userID = reallocate(userID, USER_ID_SIZE);
        pszUser = reallocate(pszUser, strlen(pszUser)+1);
        pszPwd = reallocate(pszPwd, strlen(pszPwd)+1);
        psznick = reallocate(psznick, strlen(psznick)+1);
        pExtendInfo = reallocate(pExtendInfo, ExtendLen);
    }
    void dtor() {
        release(userID);
        release(pszUser);
        release(pszPwd);
        release(psznick);
        release(pExtendInfo);
    }
    void call() {
        // 判断是否已经注册
        auto_dispose_ptr<GetAccountRequest> getAccountRequest(GetAccountRequest::New());
        getAccountRequest->set_userid(userID);
        auto_dispose_ptr<GetAccountResponse> getAccountResponse;
        if (g_pDataDomainSvc->GetAccount(getAccountRequest, &getAccountResponse) != 0) {
            SSLOG_ERROR(_T("Can't register."));
            CallbackLog(index, ID_RegistResp, Result_ConnectFail, NULL, 0);
            return;
        }
        if (getAccountResponse->has_account()) {
            SSLOG_DEBUG(_T("the user exist: ") << userID);
            CallbackLog(index, ID_RegistResp, Result_UserIsRegisted, NULL, 0);
            return;
        }
        // 创建帐号
        auto_dispose_ptr<CreateAccountRequest> createAccountRequest(CreateAccountRequest::New());
        createAccountRequest->set_userid(userID);
        createAccountRequest->set_email(pszUser);
        createAccountRequest->set_password(pszPwd);
        createAccountRequest->set_nickname(psznick);
        auto_dispose_ptr<CreateAccountResponse> createAccountResponse;
        if (g_pDataDomainSvc->CreateAccount(createAccountRequest, &createAccountResponse) != 0) {
            SSLOG_ERROR(_T("Can't create account."));
            CallbackLog(index, ID_RegistResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(createAccountResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while creating account: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_RegistResp, Result_InternalFail, NULL, 0);
            return;
        }

        CallbackLog(index, ID_RegistResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL7(index, userID, pszUser, pszPwd, psznick, pExtendInfo, ExtendLen)
    //=========================================================================

    return Result_Success;
}

SSResultCode SDLogIn(unsigned int index, const char * userID, 
                     const char * md5Pwd,
                     IUserInterface ** pIUser) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SDLogIn(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();

    if (g_login_user.compare(userID) != 0) {
        UserInterface* pUI = new UserInterface();
        pUI->m_strUserId = userID;
        pUI->m_bLogin = false;
        *pIUser = pUI;
    } else {
        *pIUser = g_pUserInterface;
    }
    
    //=========================================================================
    BEGIN_ASYNC_CALL4(unsigned int, index,
                      const char*,  userID,
                      const char*,  md5Pwd,
                      IUserInterface*, pUI)
    void ctor() {
        userID = reallocate(userID, USER_ID_SIZE);
        md5Pwd = reallocate(md5Pwd, strlen(md5Pwd)+1);
    }
    void dtor() {
        release(userID);
        release(md5Pwd);
    }
    void call() {
        if (g_login_user.compare(userID) == 0) {
            CallbackLog(index, ID_LoginResp, Result_Success, NULL, 0);
            return;
        }

        // 获取帐号
        auto_dispose_ptr<GetAccountRequest> getAccountRequest(GetAccountRequest::New());
        getAccountRequest->set_userid(userID);
        auto_dispose_ptr<GetAccountResponse> getAccountResponse;
        if (g_pDataDomainSvc->GetAccount(getAccountRequest, &getAccountResponse) != 0) {
            SSLOG_ERROR(_T("Can't get account."));
            CallbackLog(index, ID_LoginResp, Result_ConnectFail, NULL, 0);
            return;
        }
        if (!getAccountResponse->has_account()) {
            SSLOG_DEBUG(_T("user doesn't exist."));
            CallbackLog(index, ID_LoginResp, Result_PasswordError, NULL, 0);
            return;
        }
        // 验证密码
        auto_dispose_ptr<SSAccount> account(getAccountResponse->get_account());
        if (strcmp(account->get_password(), md5Pwd) != 0) {
            SSLOG_DEBUG(_T("password doesn't match."));
            CallbackLog(index, ID_LoginResp, Result_PasswordError, NULL, 0);
            return;
        }
        
        g_pUserInterface = pUI;
        g_login_user = userID;
        ((UserInterface*)pUI)->m_bLogin = true;
        CallbackLog(index, ID_LoginResp, Result_Success, NULL, 0);

        // 计算最后分享的序号
        auto_dispose_ptr<GetMessageRequest> getMessageRequest(GetMessageRequest::New());
        getMessageRequest->set_userid(userID);
        getMessageRequest->set_source_box(PUBBOX);
        getMessageRequest->set_count(1);
        auto_dispose_ptr<GetMessageResponse> getMessageResponse;
        if (g_pDataDomainSvc->GetMessage(getMessageRequest, &getMessageResponse) != 0) {
            SSLOG_ERROR(_T("Can't get last seq number."));
        } else {
            auto_dispose_ptr<ServiceResult> result(getMessageResponse->get_result());
            if (result->get_code() != SC_OK) {
                SSLOG_ERROR(_T("Error occurs while getting last seq number: ")
                    << result->get_code()
                    << _T(",")
                    << UTF8TOTCHAR(result->get_message()));
                if (result->get_code() == SC_NO_SUCH_OBJECT) {
                    if (g_last_seq_number == 0) {
                        g_last_seq_number = 1;
                    }
		            //added by tangjun 郝喜强要求得到这个last_seq_number
		            CallbackLog(index, ID_LastPubShareSeqNumMsg, Result_Success, &g_last_seq_number, 1);
                }
            } else {
                if (getMessageResponse->get_msg_list_size() > 0) {
                    auto_dispose_ptr<SSMessage> message(getMessageResponse->get_msg_list(0));
                    g_last_seq_number = message->get_seq_number();

                    if (g_last_seq_number == 0) {
                        g_last_seq_number = 1;
                    }
		            //added by tangjun 郝喜强要求得到这个last_seq_number
		            CallbackLog(index, ID_LastPubShareSeqNumMsg, Result_Success, &g_last_seq_number, 1);
                }
            }
        }

        // 登录成功后启动消息泵
        g_message_pump_station->set_userid(userID);
        g_message_pump_station->set_data_svc(g_pDataDomainSvc);
        g_message_pump_station->set_call_back(CallbackLog);
        g_message_pump_station->start();
    }
    END_ASYNC_CALL4(index, userID, md5Pwd, *pIUser)
    //=========================================================================

    return Result_Success;
}

SSResultCode SDLogOut(unsigned int index, IUserInterface * pIUser) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SDLogOut(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();

    g_login_user.clear();
    if (pIUser != g_pUserInterface && pIUser != NULL) {
        delete pIUser;
    }
    if (g_pUserInterface != NULL) {
        delete g_pUserInterface;
        g_pUserInterface = NULL;
    }

    // 退出后停止消息泵
    g_message_pump_station->stop();
    
    //=========================================================================
    //BEGIN_ASYNC_CALL1(unsigned int, index)
    //void ctor() {
    //}
    //void dtor() {
    //}
    //void call() {
    //    CallbackLog(index, ID_NULL, Result_Success, NULL, 0);
    //}
    //END_ASYNC_CALL1(index)
    //=========================================================================

    return Result_Success;
}

SSResultCode SDForgetPassword(unsigned int index, const char * userID) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SDForgetPassword(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(unsigned int, index, const char *, userID)
    void ctor() {
        userID = reallocate(userID, USER_ID_SIZE);
    }
    void dtor() {
        release(userID);
    }
    void call() {
        //获取帐号
        auto_dispose_ptr<GetAccountRequest> getAccountRequest(GetAccountRequest::New());
        getAccountRequest->set_userid(userID);
        auto_dispose_ptr<GetAccountResponse> getAccountResponse;
        if (g_pDataDomainSvc->GetAccount(getAccountRequest, &getAccountResponse) != 0) {
            SSLOG_ERROR(_T("Can't get account."));
            CallbackLog(index, ID_ForgetPwdResp, Result_ConnectFail, NULL, 0);
            return;
        }
        {auto_dispose_ptr<ServiceResult> result(getAccountResponse->get_result());
        if (result->get_code() == SC_NO_SUCH_OBJECT) {
            SSLOG_ERROR(_T("the user doesn't exist."));
            CallbackLog(index, ID_ForgetPwdResp, Result_UserNotExist, NULL, 0);
            return;
        }
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting account: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_ForgetPwdResp, Result_InternalFail, NULL, 0);
            return;
        }}
        static char table[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        char code[33];
        int64 time;
        srand((unsigned int)::time(NULL));
        for (int i = 0; i < 32; i++) {
            code[i] = table[rand()%(sizeof(table)/sizeof(table[0]))];
        }
        code[32] = '\0';
        time = ::_time64(NULL);
        
        auto_dispose_ptr<SetSecurityCodeRequest> setSecurityCodeRequest(SetSecurityCodeRequest::New());
        setSecurityCodeRequest->set_userid(userID);
        setSecurityCodeRequest->set_code(code);
        setSecurityCodeRequest->set_time(time);
        auto_dispose_ptr<SetSecurityCodeResponse> setSecurityCodeResponse;
        if (g_pDataDomainSvc->SetSecurityCode(setSecurityCodeRequest, &setSecurityCodeResponse) != 0) {
            SSLOG_ERROR(_T("Can't set security code."));
            CallbackLog(index, ID_ForgetPwdResp, Result_ConnectFail, NULL, 0);
            return;
        }
        {auto_dispose_ptr<ServiceResult> result(setSecurityCodeResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while setting security code: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_ForgetPwdResp, Result_InternalFail, NULL, 0);
            return;
        }}
        //发送验证码邮件
        auto_dispose_ptr<SSAccount> account(getAccountResponse->get_account());
        SSLOG_DEBUG(_T("email address: ") << account->get_email());
        SSLOG_DEBUG(_T("security code: ") << code);
        CallbackLog(index, ID_ForgetPwdResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL2(index, userID)
    //=========================================================================

    return Result_Success;
}

SSResultCode SDVerifyActiveCode(unsigned int index, const char * userID, const char * pszVerifyCode) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SDVerifyActiveCode(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(unsigned int, index, const char *, userID, const char *, pszVerifyCode)
    void ctor() {
        userID = reallocate(userID, USER_ID_SIZE);
        pszVerifyCode = reallocate(pszVerifyCode, strlen(pszVerifyCode)+1);
    }
    void dtor() {
        release(userID);
        release(pszVerifyCode);
    }
    void call() {
        SSLOG_DEBUG(_T("verify code：") << pszVerifyCode);
        auto_dispose_ptr<GetSecurityCodeRequest> getSecurityCodeRequest(GetSecurityCodeRequest::New());
        getSecurityCodeRequest->set_userid(userID);
        auto_dispose_ptr<GetSecurityCodeResponse> getSecurityCodeResponse;
        if (g_pDataDomainSvc->GetSecurityCode(getSecurityCodeRequest, &getSecurityCodeResponse) != 0) {
            SSLOG_ERROR(_T("Can't get security code."));
            CallbackLog(index, ID_VerifyActiveCodeResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getSecurityCodeResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting security code: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_VerifyActiveCodeResp, Result_InternalFail, NULL, 0);
            return;
        }

        const char * code = getSecurityCodeResponse->get_code();
        int64 time = getSecurityCodeResponse->get_time();
        int64 curr = _time64(NULL);
        
        SSLOG_DEBUG(_T("security code：") << code);

        if (time + 30*60*1000 < curr) {
            SSLOG_ERROR(_T("Security code is timeout."));
            CallbackLog(index, ID_VerifyActiveCodeResp, Result_VerifyActiveCodeTimeOut, NULL, 0);
            return;
        }
        if (strcmp(code, pszVerifyCode) != 0) {
            SSLOG_ERROR(_T("Security code doesn't match."));
            CallbackLog(index, ID_VerifyActiveCodeResp, Result_VerifyActiveCode, NULL, 0);
            return;
        }
        CallbackLog(index, ID_VerifyActiveCodeResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(index, userID, pszVerifyCode)
    //=========================================================================

    return Result_Success;
}

SSResultCode SDChangePassword(unsigned int index, const char * userID, const char * newPwd) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SDChangePassword(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(unsigned int, index, const char *, userID, const char *, newPwd)
    void ctor() {
        userID = reallocate(userID, USER_ID_SIZE);
        newPwd = reallocate(newPwd, strlen(newPwd)+1);
    }
    void dtor() {
        release(userID);
        release(newPwd);
    }
    void call() {
        // 修改密码
        auto_dispose_ptr<ChangePasswordRequest> changePasswordRequest(ChangePasswordRequest::New());
        changePasswordRequest->set_userid(userID);
        changePasswordRequest->set_new_password(newPwd);
        auto_dispose_ptr<ChangePasswordResponse> changePasswordResponse;
        if (g_pDataDomainSvc->ChangePassword(changePasswordRequest, &changePasswordResponse) != 0) {
            SSLOG_ERROR(_T("Can't change password."));
            CallbackLog(index, ID_ChangePwdResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(changePasswordResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while changing password: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_ChangePwdResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_ChangePwdResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(index, userID, newPwd)
    //=========================================================================

    return Result_Success;
}

SSResultCode SDUserIsExist(unsigned int index, const char* userIDs) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SDUserIsExist(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(unsigned int, index, const char*, userIDs)
    void ctor() {
        userIDs = reallocate(userIDs, strlen(userIDs)+1);
    }
    void dtor() {
        release(userIDs);
    }
    void call() {
        auto_dispose_ptr<UserExistRequest> userExistRequest(UserExistRequest::New());
        StringSplit split(userIDs, ",");
        while (!split.eof()) {
            userExistRequest->add_userid_list(split.next());
        }

        // 初始化返回值
        int count = userExistRequest->get_userid_list_size();
        std::unique_ptr<SSUserExistT[]> exist_user_list(new SSUserExistT[count]);
        for (int i = 0; i < count; i++) {
            strncpy(exist_user_list[i].userId, userExistRequest->get_userid_list(i), USER_ID_LENGTH);
            exist_user_list[i].isSystem = false;
        }
        // 验证用户是否存在
        auto_dispose_ptr<UserExistResponse> userExistResponse;
        if (g_pDataDomainSvc->UserExist(userExistRequest, &userExistResponse) != 0) {
            SSLOG_ERROR(_T("Can't call UserExist."));
            CallbackLog(index, ID_UserExistResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(userExistResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while checking user exist: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_UserExistResp, Result_InternalFail, NULL, 0);
            return;
        }
        // 返回结果
        for (int i = 0; i < userExistResponse->get_exist_userid_list_size(); i++) {
            for (int j = 0; j < count; j++) {
                if (strncmp(userExistResponse->get_exist_userid_list(i),
                            exist_user_list[j].userId,
                            USER_ID_LENGTH) == 0) {
                    exist_user_list[j].isSystem = true;
                    break;
                }
            }
        }
        CallbackLog(index, ID_UserExistResp, Result_Success, exist_user_list.get(), count);
    }
    END_ASYNC_CALL2(index, userIDs)
    //=========================================================================

    return Result_Success;
}

///////////////////////////////////////////////////////////////////////////////

SSResultCode UserInterface::VerifyActiveCode(unsigned int index, const char * pszVerifyCode) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("UserInterface(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, const char*, pszVerifyCode)
    void ctor() {
        pszVerifyCode = reallocate(pszVerifyCode, strlen(pszVerifyCode)+1);
    }
    void dtor() {
        release(pszVerifyCode);
    }
    void call() {
        CallbackLog(index, ID_VerifyActiveCodeResp, Result_FunctionNoImplement, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, pszVerifyCode)
    //=========================================================================

    return Result_Success;
}

SSResultCode UserInterface::ChangePassword(unsigned int index,const char * newPwd) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("ChangePassword(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, const char*, newPwd)
    void ctor() {
        newPwd = reallocate(newPwd, strlen(newPwd)+1);
    }
    void dtor() {
        release(newPwd);
    }
    void call() {
        // 修改密码
        auto_dispose_ptr<ChangePasswordRequest> changePasswordRequest(ChangePasswordRequest::New());
        changePasswordRequest->set_userid(ui->m_strUserId.c_str());
        changePasswordRequest->set_new_password(newPwd);
        auto_dispose_ptr<ChangePasswordResponse> changePasswordResponse;
        if (g_pDataDomainSvc->ChangePassword(changePasswordRequest, &changePasswordResponse) != 0) {
            SSLOG_ERROR(_T("Can't change password."));
            CallbackLog(index, ID_ChangePwdResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(changePasswordResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while changing password: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_ChangePwdResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_ChangePwdResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, newPwd)
    //=========================================================================

    return Result_Success;
}

SSResultCode UserInterface::GetProfile(unsigned int index) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetProfile(") << index << _T(")"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(UserInterface*, ui, unsigned int, index)
    void ctor() {
    }
    void dtor() {
    }
    void call() {
        // 获取个人设置
        auto_dispose_ptr<GetProfileRequest> getProfileRequest(GetProfileRequest::New());
        getProfileRequest->add_userid_list(ui->m_strUserId.c_str());
        //getProfileRequest->set_flag(1); // 除icon之外的所有信息
        auto_dispose_ptr<GetProfileResponse> getProfileResponse;
        if (g_pDataDomainSvc->GetProfile(getProfileRequest, &getProfileResponse) != 0) {
            SSLOG_ERROR(_T("Can't get profile."));
            CallbackLog(index, ID_ProfileResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getProfileResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting profile: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_ProfileResp, Result_InternalFail, NULL, 0);
            return;
        }
        // 转换profile
        int count = getProfileResponse->get_profile_list_size();
        std::unique_ptr<SSProfileT[]> data(new SSProfileT[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSProfile> profile(getProfileResponse->get_profile_list(i));
            convert(&data[i], profile);
        }
        CallbackLog(index, ID_ProfileResp, Result_Success, data.get(), count);
    }
    END_ASYNC_CALL2(this, index)
    //=========================================================================

    return Result_Success;
}

SSResultCode UserInterface::SetProfile(unsigned int index, const ::SSProfileT& userProfile) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SetProfile(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, SSProfileT*, userProfile)
    void ctor() {
        userProfile = reallocate(userProfile, sizeof(SSProfileT));
        if (userProfile->extend != NULL) {
            userProfile->extend = reallocate(userProfile->extend, userProfile->LengthEx);
        }
    }
    void dtor() {
        release(userProfile);
    }
    void call() {
        // 设置Profile
        auto_dispose_ptr<SetProfileRequest> setProfileRequest(SetProfileRequest::New());
        setProfileRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<SSProfile> profile(SSProfile::New());
        convert(profile, userProfile);
        if (!profile->has_userid())
            profile->set_userid(ui->m_strUserId.c_str());
        setProfileRequest->set_profile(profile);
        auto_dispose_ptr<SetProfileResponse> setProfileResponse;
        if (g_pDataDomainSvc->SetProfile(setProfileRequest, &setProfileResponse) != 0) {
            SSLOG_ERROR(_T("Can't set profile."));
            CallbackLog(index, ID_ProfileResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(setProfileResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while setting profile: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_ProfileResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_ProfileResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (SSProfileT*)&userProfile)
    //=========================================================================

    return Result_Success;
}

SSResultCode UserInterface::GetOtherProfile(unsigned int index, const char* userIDs) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetOtherProfile(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, const char*, userIDs)
    void ctor() {
        userIDs = reallocate(userIDs, strlen(userIDs)+1);
    }
    void dtor() {
        release(userIDs);
    }
    void call() {
        // 获取个人设置
        auto_dispose_ptr<GetProfileRequest> getProfileRequest(GetProfileRequest::New());
        StringSplit split(userIDs, ",");
        while (!split.eof()) {
            getProfileRequest->add_userid_list(split.next());
        }
        getProfileRequest->set_flag(1); // 除icon之外的所有信息
        auto_dispose_ptr<GetProfileResponse> getProfileResponse;
        if (g_pDataDomainSvc->GetProfile(getProfileRequest, &getProfileResponse) != 0) {
            SSLOG_ERROR(_T("Can't get other profile."));
            CallbackLog(index, ID_OtherUserProfileResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getProfileResponse->get_result());

		//added by tangjun 应郝喜强要求，当用户不存在的时候返回Result_UserNotExist
		if(result->get_code() == SC_NO_SUCH_OBJECT)
		{
			CallbackLog(index, ID_OtherUserProfileResp, Result_UserNotExist, NULL, 0);
			return;		
		}

        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting other profile: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_OtherUserProfileResp, Result_InternalFail, NULL, 0);
            return;
        }
        // 转换profile
        int count = getProfileResponse->get_profile_list_size();

		//added by tangjun 应郝喜强要求，当用户不存在的时候返回Result_UserNotExist
		if(count == 0)
		{
			CallbackLog(index, ID_OtherUserProfileResp, Result_UserNotExist, NULL, 0);
			return;
		}

        std::unique_ptr<SSProfileT[]> data(new SSProfileT[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSProfile> profile(getProfileResponse->get_profile_list(i));
            convert(&data[i], profile);
        }
        CallbackLog(index, ID_OtherUserProfileResp, Result_Success, data.get(), count);
    }
    END_ASYNC_CALL3(this, index, userIDs)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetIcons(unsigned int index, const char* userIDs) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetIcons(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, char*, userIDs)
    void ctor() {
        userIDs = reallocate(userIDs, strlen(userIDs)+1);
    }
    void dtor() {
        release(userIDs);
    }
    void call() {
        // 获取个人设置
        auto_dispose_ptr<GetProfileRequest> getProfileRequest(GetProfileRequest::New());
        StringSplit split(userIDs, ",");
        while (!split.eof()) {
            getProfileRequest->add_userid_list(split.next());
        }
        getProfileRequest->set_flag(0); // 只获取头像信息
        auto_dispose_ptr<GetProfileResponse> getProfileResponse;
        if (g_pDataDomainSvc->GetProfile(getProfileRequest, &getProfileResponse) != 0) {
            SSLOG_ERROR(_T("Can't get icons."));
            CallbackLog(index, ID_IconResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getProfileResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting icons: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_IconResp, Result_InternalFail, NULL, 0);
            return;
        }
        // 转换profile
        int count = getProfileResponse->get_profile_list_size();
        std::unique_ptr<SSIconT[]> data(new SSIconT[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSProfile> profile(getProfileResponse->get_profile_list(i));
            SSProfileT profile_t;
            convert(&profile_t, profile);
            memcpy(data[i].userid, profile_t.userid, USER_ID_SIZE);
            memcpy(data[i].iconname, profile_t.iconname, MAX_STR_LENGTH);
            memcpy(data[i].icon, profile_t.icon, MAX_ICON_LENGTH);
            data[i].LengthIcon = profile_t.LengthIcon;
        }
        for (int i = 0; i < count; i++) {
            CallbackLog(index, ID_IconResp, Result_Success, &data[i], 1);
        }
    }
    END_ASYNC_CALL3(this, index, (char*)userIDs)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::SetIcon(unsigned int index, const ::SSIconT& userIcon) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SetIcon(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();

    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, SSIconT*, userIcon)
    void ctor() {
        userIcon = reallocate(userIcon, sizeof(SSIconT));
    }
    void dtor() {
        release(userIcon);
    }
    void call() {
        // SSIconT转成SSProfileT，然后设置Profile
        auto_dispose_ptr<SetProfileRequest> setProfileRequest(SetProfileRequest::New());
        setProfileRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<SSProfile> profile(SSProfile::New());
        SSProfileT userProfile;
        
        memcpy(userProfile.userid, userIcon->userid, USER_ID_SIZE);
        memcpy(userProfile.icon, userIcon->icon, MAX_ICON_LENGTH);
        memcpy(userProfile.iconname, userIcon->iconname, MAX_STR_LENGTH);
        userProfile.LengthIcon = userIcon->LengthIcon;
        convert(profile, &userProfile);
        if (!profile->has_userid())
            profile->set_userid(ui->m_strUserId.c_str());
        setProfileRequest->set_profile(profile);
        auto_dispose_ptr<SetProfileResponse> setProfileResponse;
        if (g_pDataDomainSvc->SetProfile(setProfileRequest, &setProfileResponse) != 0) {
            SSLOG_ERROR(_T("Can't set icon."));
            CallbackLog(index, ID_IconResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(setProfileResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while setting icon: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_IconResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_IconResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (SSIconT*)&userIcon)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetFollowCount(unsigned int index) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetFollowCount(") << index << _T(")"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(UserInterface*, ui, unsigned int, index)
    void ctor() {
    }
    void dtor() {
    }
    void call() {
        auto_dispose_ptr<GetFollowerCountRequest> getFollowerCountRequest(GetFollowerCountRequest::New());
        getFollowerCountRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<GetFollowerCountResponse> getFollowerCountResponse;
        if (g_pDataDomainSvc->GetFollowerCount(getFollowerCountRequest, &getFollowerCountResponse) != 0) {
            SSLOG_ERROR(_T("Can't get follower count."));
            CallbackLog(index, ID_FollowCountResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getFollowerCountResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting follower count: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_FollowCountResp, Result_InternalFail, NULL, 0);
            return;
        }
        int count = getFollowerCountResponse->get_count();
        CallbackLog(index, ID_FollowCountResp, Result_Success, &count, 1);
    }
    END_ASYNC_CALL2(this, index)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetFollowingCount(unsigned int index) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetFollowingCount(") << index << _T(")"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(UserInterface*, ui, unsigned int, index)
    void ctor() {
    }
    void dtor() {
    }
    void call() {
        auto_dispose_ptr<GetFollowingCountRequest> getFollowingCountRequest(GetFollowingCountRequest::New());
        getFollowingCountRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<GetFollowingCountResponse> getFollowingCountResponse;
        if (g_pDataDomainSvc->GetFollowingCount(getFollowingCountRequest, &getFollowingCountResponse) != 0) {
            SSLOG_ERROR(_T("Can't get following count."));
            CallbackLog(index, ID_FollowingCountResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getFollowingCountResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting following count: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_FollowingCountResp, Result_InternalFail, NULL, 0);
            return;
        }
        int count = getFollowingCountResponse->get_count();
        CallbackLog(index, ID_FollowingCountResp, Result_Success, &count, 1);
    }
    END_ASYNC_CALL2(this, index)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetFollowList(unsigned int index, const char * lastID, int getCount) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetFollowList(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL4(UserInterface*, ui, unsigned int, index, char*, lastID, int, count)
    void ctor() {
        if (lastID != NULL)
            lastID = reallocate(lastID, strlen(lastID)+1);
    }
    void dtor() {
        if (lastID != NULL)
            release(lastID);
    }
    void call() {
        auto_dispose_ptr<GetFollowerWithProfileRequest> getFollowerWithProfileRequest(GetFollowerWithProfileRequest::New());
        getFollowerWithProfileRequest->set_userid(ui->m_strUserId.c_str());
        getFollowerWithProfileRequest->set_exclude_icon(true);
        if (lastID != NULL)
            getFollowerWithProfileRequest->set_cursor_id(lastID);
        getFollowerWithProfileRequest->set_count(count);
        auto_dispose_ptr<GetFollowerWithProfileResponse> getFollowerWithProfileResponse;
        if (g_pDataDomainSvc->GetFollowerWithProfile(getFollowerWithProfileRequest, &getFollowerWithProfileResponse) != 0) {
            SSLOG_ERROR(_T("Can't get follower list."));
            CallbackLog(index, ID_FollowListResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getFollowerWithProfileResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting following list: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_FollowListResp, Result_InternalFail, NULL, 0);
            return;
        }
        // 转换profile
        int count = getFollowerWithProfileResponse->get_follower_list_size();
        std::unique_ptr<SSProfileT[]> data(new SSProfileT[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSProfile> profile(getFollowerWithProfileResponse->get_follower_list(i));
            convert(&data[i], profile);
        }
        CallbackLog(index, ID_FollowListResp, Result_Success, data.get(), count);
    }
    END_ASYNC_CALL4(this, index, (char*)lastID, getCount)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetFollowingList(unsigned int index) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetFollowingList(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(UserInterface*, ui, unsigned int, index)
    void ctor() {
    }
    void dtor() {
    }
    void call() {
        auto_dispose_ptr<GetFollowingWithProfileRequest> getFollowingWithProfileRequest(GetFollowingWithProfileRequest::New());
        getFollowingWithProfileRequest->set_userid(ui->m_strUserId.c_str());
        getFollowingWithProfileRequest->set_exclude_icon(true);
        auto_dispose_ptr<GetFollowingWithProfileResponse> getFollowingWithProfileResponse;
        if (g_pDataDomainSvc->GetFollowingWithProfile(getFollowingWithProfileRequest, &getFollowingWithProfileResponse) != 0) {
            SSLOG_ERROR(_T("Can't get following list."));
            CallbackLog(index, ID_FollowingListResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getFollowingWithProfileResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting following list: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_FollowingListResp, Result_InternalFail, NULL, 0);
            return;
        }
        // 转换profile
        int count = getFollowingWithProfileResponse->get_following_list_size();
        std::unique_ptr<SSProfileT[]> data(new SSProfileT[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSProfile> profile(getFollowingWithProfileResponse->get_following_list(i));
            convert(&data[i], profile);
        }
        CallbackLog(index, ID_FollowingListResp, Result_Success, data.get(), count);
    }
    END_ASYNC_CALL2(this, index)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetOtherFollowCount(unsigned int index, const char * otherUserID) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetOtherFollowCount(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, char*, otherUserID)
    void ctor() {
        otherUserID = reallocate(otherUserID, USER_ID_SIZE);
    }
    void dtor() {
        release(otherUserID);
    }
    void call() {
        auto_dispose_ptr<GetFollowerCountRequest> getFollowerCountRequest(GetFollowerCountRequest::New());
        getFollowerCountRequest->set_userid(otherUserID);
        auto_dispose_ptr<GetFollowerCountResponse> getFollowerCountResponse;
        if (g_pDataDomainSvc->GetFollowerCount(getFollowerCountRequest, &getFollowerCountResponse) != 0) {
            SSLOG_ERROR(_T("Can't get other follower count."));
            CallbackLog(index, ID_OtherFollowCountResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getFollowerCountResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting other follower count: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_OtherFollowCountResp, Result_InternalFail, NULL, 0);
            return;
        }
        int count = getFollowerCountResponse->get_count();
        CallbackLog(index, ID_OtherFollowCountResp, Result_Success, &count, 1);
    }
    END_ASYNC_CALL3(this, index, (char*)otherUserID)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetOtherFollowingCount(unsigned int index, const char * otherUserID) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetOtherFollowingCount(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, char*, otherUserID)
    void ctor() {
        otherUserID = reallocate(otherUserID, strlen(otherUserID)+1);
    }
    void dtor() {
        release(otherUserID);
    }
    void call() {
        auto_dispose_ptr<GetFollowingCountRequest> getFollowingCountRequest(GetFollowingCountRequest::New());
        getFollowingCountRequest->set_userid(otherUserID);
        auto_dispose_ptr<GetFollowingCountResponse> getFollowingCountResponse;
        if (g_pDataDomainSvc->GetFollowingCount(getFollowingCountRequest, &getFollowingCountResponse) != 0) {
            SSLOG_ERROR(_T("Can't get other following count."));
            CallbackLog(index, ID_OtherFollowingCountResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getFollowingCountResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting other following count: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_OtherFollowingCountResp, Result_InternalFail, NULL, 0);
            return;
        }
        int count = getFollowingCountResponse->get_count();
        CallbackLog(index, ID_OtherFollowingCountResp, Result_Success, &count, 1);
    }
    END_ASYNC_CALL3(this, index, (char*)otherUserID)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetOtherFollowList(unsigned int index, const char* lastID, int getCount,const char * userID) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetOtherFollowList(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL5(UserInterface*, ui, unsigned int, index, char*, lastID, int, count, char*, userID)
    void ctor() {
        if (lastID != NULL)
            lastID = reallocate(lastID, strlen(lastID)+1);
        userID = reallocate(userID, strlen(userID)+1);
    }
    void dtor() {
        if (lastID != NULL)
            release(lastID);
        release(userID);
    }
    void call() {
        auto_dispose_ptr<GetFollowerWithProfileRequest> getFollowerWithProfileRequest(GetFollowerWithProfileRequest::New());
        getFollowerWithProfileRequest->set_userid(userID);
        getFollowerWithProfileRequest->set_exclude_icon(true);
        if (lastID != NULL)
            getFollowerWithProfileRequest->set_cursor_id(lastID);
        getFollowerWithProfileRequest->set_count(count);
        auto_dispose_ptr<GetFollowerWithProfileResponse> getFollowerWithProfileResponse;
        if (g_pDataDomainSvc->GetFollowerWithProfile(getFollowerWithProfileRequest, &getFollowerWithProfileResponse) != 0) {
            SSLOG_ERROR(_T("Can't get other follower list."));
            CallbackLog(index, ID_OtherFollowListResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getFollowerWithProfileResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting other following list: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_OtherFollowListResp, Result_InternalFail, NULL, 0);
            return;
        }
        // 转换profile
        int count = getFollowerWithProfileResponse->get_follower_list_size();
        std::unique_ptr<SSProfileT[]> data(new SSProfileT[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSProfile> profile(getFollowerWithProfileResponse->get_follower_list(i));
            convert(&data[i], profile);
        }
        CallbackLog(index, ID_OtherFollowListResp, Result_Success, data.get(), count);
    }
    END_ASYNC_CALL5(this, index, (char*)lastID, getCount, (char*)userID)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetOtherFollowingList(unsigned int index, const char * userID) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetOtherFollowingList(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, char*, userID)
    void ctor() {
        userID = reallocate(userID, strlen(userID)+1);
    }
    void dtor() {
        release(userID);
    }
    void call() {
        auto_dispose_ptr<GetFollowingWithProfileRequest> getFollowingWithProfileRequest(GetFollowingWithProfileRequest::New());
        getFollowingWithProfileRequest->set_userid(userID);
        getFollowingWithProfileRequest->set_exclude_icon(true);
        auto_dispose_ptr<GetFollowingWithProfileResponse> getFollowingWithProfileResponse;
        if (g_pDataDomainSvc->GetFollowingWithProfile(getFollowingWithProfileRequest, &getFollowingWithProfileResponse) != 0) {
            SSLOG_ERROR(_T("Can't get other following list."));
            CallbackLog(index, ID_OtherFollowingListResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getFollowingWithProfileResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting other following list: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_OtherFollowingListResp, Result_InternalFail, NULL, 0);
            return;
        }
        // 转换profile
        int count = getFollowingWithProfileResponse->get_following_list_size();
        std::unique_ptr<SSProfileT[]> data(new SSProfileT[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSProfile> profile(getFollowingWithProfileResponse->get_following_list(i));
            convert(&data[i], profile);
        }
        CallbackLog(index, ID_OtherFollowingListResp, Result_Success, data.get(), count);
    }
    END_ASYNC_CALL3(this, index, (char*)userID)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetContacts (unsigned int index) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetContacts(") << _T(")"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(UserInterface*, ui, unsigned int, index)
    void ctor() {
    }
    void dtor() {
    }
    void call() {
        auto_dispose_ptr<GetContactExRequest> getContactExRequest(GetContactExRequest::New());
        getContactExRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<GetContactExResponse> getContactExResponse;
        if (g_pDataDomainSvc->GetContactEx(getContactExRequest, &getContactExResponse) != 0) {
            SSLOG_ERROR(_T("Can't get contact."));
            CallbackLog(index, ID_ContactsResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getContactExResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting contact: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_ContactsResp, Result_InternalFail, NULL, 0);
            return;
        }
        int count = getContactExResponse->get_contact_ex_list_size();
        std::unique_ptr<SSContactsT[]> data(new SSContactsT[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSContactEx> contact_ex(getContactExResponse->get_contact_ex_list(i));
            convert(&data[i], contact_ex);
        }
        CallbackLog(index, ID_ContactsResp, Result_Success, data.get(), count);
    }
    END_ASYNC_CALL2(this, index)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::SetContact(unsigned int index, const ::SSContactsExT & contact) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SetContact(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, SSContactsExT*, contact)
    void ctor() {
        contact = reallocate(contact, sizeof(SSContactsExT));
    }
    void dtor() {
        release(contact);
    }
    void call() {
        auto_dispose_ptr<AddContactRequest> addContactRequest(AddContactRequest::New());
        addContactRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<SSContact> sscontact(SSContact::New());
        convert(sscontact, contact);
        addContactRequest->add_contact_list(sscontact);
        auto_dispose_ptr<AddContactResponse> addContactResponse;
        if (g_pDataDomainSvc->AddContact(addContactRequest, &addContactResponse) != 0) {
            SSLOG_ERROR(_T("Can't set contact."));
            CallbackLog(index, ID_SetContactResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(addContactResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while setting contact: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_SetContactResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_SetContactResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (SSContactsExT*)&contact)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::AddContacts(unsigned int index, const ::SSContactsExT* contacts, unsigned int length) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("AddContacts(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL4(UserInterface*, ui, unsigned int, index, SSContactsExT*, contacts, unsigned int, length)
    void ctor() {
         contacts = reallocate(contacts, length * sizeof(SSContactsExT));
    }
    void dtor() {
        release(contacts);
    }
    void call() {
        auto_dispose_ptr<AddContactRequest> addContactRequest(AddContactRequest::New());
        addContactRequest->set_userid(ui->m_strUserId.c_str());
        for (unsigned int i = 0; i < length; i++) {
            auto_dispose_ptr<SSContact> sscontact(SSContact::New());
            convert(sscontact, &contacts[i]);
            addContactRequest->add_contact_list(sscontact);
        }
        auto_dispose_ptr<AddContactResponse> addContactResponse;
        if (g_pDataDomainSvc->AddContact(addContactRequest, &addContactResponse) != 0) {
            SSLOG_ERROR(_T("Can't add contact."));
            CallbackLog(index, ID_AddContactsResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(addContactResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while adding contact: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_AddContactsResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_AddContactsResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL4(this, index, (SSContactsExT*)contacts, length)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::RemoveContacts(unsigned int index, const char* contactIDs) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("RemoveContacts(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, char*, contactIDs)
    void ctor() {
        contactIDs = reallocate(contactIDs, strlen(contactIDs)+1);
    }
    void dtor() {
        release(contactIDs);
    }
    void call() {
        auto_dispose_ptr<RemoveContactRequest> removeContactRequest(RemoveContactRequest::New());
        removeContactRequest->set_userid(ui->m_strUserId.c_str());
        StringSplit split(contactIDs, ",");
        while (!split.eof()) {
            removeContactRequest->add_contactid_list(split.next());
        }
        auto_dispose_ptr<RemoveContactResponse> removeContactResponse;
        if (g_pDataDomainSvc->RemoveContact(removeContactRequest, &removeContactResponse) != 0) {
            SSLOG_ERROR(_T("Can't remove contact."));
            CallbackLog(index, ID_RemoveContactsResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(removeContactResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while removing contact: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_RemoveContactsResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_RemoveContactsResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (char*)contactIDs)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetGroups(unsigned int index) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetGroups(") << _T(")"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(UserInterface*, ui, unsigned int, index)
    void ctor() {
    }
    void dtor() {
    }
    void call() {
        auto_dispose_ptr<GetGroupRequest> getGroupRequest(GetGroupRequest::New());
        getGroupRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<GetGroupResponse> getGroupResponse;
        if (g_pDataDomainSvc->GetGroup(getGroupRequest, &getGroupResponse) != 0) {
            SSLOG_ERROR(_T("Can't get group."));
            CallbackLog(index, ID_GroupResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getGroupResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting group: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_GroupResp, Result_InternalFail, NULL, 0);
            return;
        }
        int count = getGroupResponse->get_group_list_size();
        std::unique_ptr<SSGroupT[]> data(new SSGroupT[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSGroup> group(getGroupResponse->get_group_list(i));
            convert(&data[i], group);
        }
        CallbackLog(index, ID_GroupResp, Result_Success, data.get(), count);
    }
    END_ASYNC_CALL2(this, index)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::SetGroup(unsigned int index, const ::SSGroupT &group) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SetGroup(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, SSGroupT*, group)
    void ctor() {
        group = reallocate(group, sizeof(SSGroupT));
        if (group->extend != NULL) {
            group->extend = reallocate(group->extend, group->LengthEx);
        }
    }
    void dtor() {
        release(group);
    }
    void call() {
        auto_dispose_ptr<AddGroupRequest> addGroupRequest(AddGroupRequest::New());
        addGroupRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<SSGroup> ssgroup(SSGroup::New());
        convert(ssgroup, group);
        addGroupRequest->add_group_list(ssgroup);
        auto_dispose_ptr<AddGroupResponse> addGroupResponse;
        if (g_pDataDomainSvc->AddGroup(addGroupRequest, &addGroupResponse) != 0) {
            SSLOG_ERROR(_T("Can't set group."));
            CallbackLog(index, ID_SetGroupResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(addGroupResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while setting group: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_SetGroupResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_SetGroupResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (SSGroupT*)&group)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::AddGroup(unsigned int index, const ::SSGroupT & group) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("AddGroup(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, SSGroupT*, group)
    void ctor() {
        group = reallocate(group, sizeof(SSGroupT));
        if (group->extend != NULL) {
            group->extend = reallocate(group->extend, group->LengthEx);
        }
    }
    void dtor() {
        release(group);
    }
    void call() {
        auto_dispose_ptr<AddGroupRequest> addGroupRequest(AddGroupRequest::New());
        addGroupRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<SSGroup> ssgroup(SSGroup::New());
        convert(ssgroup, group);
        addGroupRequest->add_group_list(ssgroup);
        auto_dispose_ptr<AddGroupResponse> addGroupResponse;
        if (g_pDataDomainSvc->AddGroup(addGroupRequest, &addGroupResponse) != 0) {
            SSLOG_ERROR(_T("Can't add group."));
            CallbackLog(index, ID_AddGroupResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(addGroupResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while adding group: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_AddGroupResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_AddGroupResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (SSGroupT*)&group)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::RemoveGroup(unsigned int index, const char* groupIDs) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("RemoveGroup(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, char*, groupIDs)
    void ctor() {
        groupIDs = reallocate(groupIDs, strlen(groupIDs)+1);
    }
    void dtor() {
        release(groupIDs);
    }
    void call() {
        auto_dispose_ptr<RemoveGroupRequest> removeGroupRequest(RemoveGroupRequest::New());
        removeGroupRequest->set_userid(ui->m_strUserId.c_str());
        StringSplit split(groupIDs, ",");
        while (!split.eof()) {
            removeGroupRequest->add_groupid_list(split.next());
        }
        auto_dispose_ptr<RemoveGroupResponse> removeGroupResponse;
        if (g_pDataDomainSvc->RemoveGroup(removeGroupRequest, &removeGroupResponse) != 0) {
            SSLOG_ERROR(_T("Can't remove group."));
            CallbackLog(index, ID_RemoveGroupResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(removeGroupResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while removing group: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_RemoveGroupResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_RemoveGroupResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (char*)groupIDs)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetLastTime(unsigned int index) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetLastTime(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(UserInterface*, ui, unsigned int, index)
    void ctor() {
    }
    void dtor() {
    }
    void call() {
        auto_dispose_ptr<GetSessionInfoRequest> getSessionInfoRequest(GetSessionInfoRequest::New());
        getSessionInfoRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<GetSessionInfoResponse> getSessionInfoResponse;
        if (g_pDataDomainSvc->GetSessionInfo(getSessionInfoRequest, &getSessionInfoResponse) != 0) {
            SSLOG_ERROR(_T("Can't get last time."));
            CallbackLog(index, ID_GetLastTimeResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getSessionInfoResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting last time: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_GetLastTimeResp, Result_InternalFail, NULL, 0);
            return;
        }
        if (!getSessionInfoResponse->has_session_info()) {
            CallbackLog(index, ID_GetLastTimeResp, Result_Success, NULL, 0);
            return;
        }
        auto_dispose_ptr<SSSessionInfo> session_info(getSessionInfoResponse->get_session_info());
        if (!session_info->has_client_state()) {
            CallbackLog(index, ID_GetLastTimeResp, Result_Success, NULL, 0);
            return;
        }
        auto_dispose_ptr<SSClientState> client_state(session_info->get_client_state());
        std::unique_ptr<SSLastTimeT> last_time(new SSLastTimeT());
        convert(last_time.get(), client_state);
        CallbackLog(index, ID_GetLastTimeResp, Result_Success, last_time.get(), 1);
    }
    END_ASYNC_CALL2(this, index)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::SetLastTime(unsigned int index, const ::SSLastTimeT& lasttime) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("SetLastTime(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, SSLastTimeT*, lasttime)
    void ctor() {
        lasttime = reallocate(lasttime, sizeof(SSLastTimeT));
    }
    void dtor() {
        release(lasttime);
    }
    void call() {
        auto_dispose_ptr<SetSessionInfoRequest> setSessionInfoRequest(SetSessionInfoRequest::New());
        setSessionInfoRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<SSSessionInfo> session_info(SSSessionInfo::New());
        auto_dispose_ptr<SSClientState> client_state(SSClientState::New());
        convert(client_state, lasttime);
        session_info->set_userid(ui->m_strUserId.c_str());
        session_info->set_client_state(client_state);
        setSessionInfoRequest->set_session_info(session_info);
        auto_dispose_ptr<SetSessionInfoResponse> setSessionInfoResponse;
        if (g_pDataDomainSvc->SetSessionInfo(setSessionInfoRequest, &setSessionInfoResponse) != 0) {
            SSLOG_ERROR(_T("Can't set last time."));
            CallbackLog(index, ID_SetLastTimeResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(setSessionInfoResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while setting last time: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_SetLastTimeResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_SetLastTimeResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (SSLastTimeT*)&lasttime)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetGroupMembers(unsigned int index) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetGroupMembers(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(UserInterface*, ui, unsigned int, index)
    void ctor() {
    }
    void dtor() {
    }
    void call() {
        auto_dispose_ptr<GetMemberRequest> getMemberRequest(GetMemberRequest::New());
        getMemberRequest->set_userid(ui->m_strUserId.c_str());
        auto_dispose_ptr<GetMemberResponse> getMemberResponse;
        if (g_pDataDomainSvc->GetMember(getMemberRequest, &getMemberResponse) != 0) {
            SSLOG_ERROR(_T("Can't get member."));
            CallbackLog(index, ID_GroupMembersResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getMemberResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting member: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_GroupMembersResp, Result_InternalFail, NULL, 0);
            return;
        }
        // 计算总数
        int count = 0;
        for (int n = 0; n < getMemberResponse->get_member_list_size(); n++) {
            auto_dispose_ptr<SSMember> members(getMemberResponse->get_member_list(n));
            count += members->get_members_size();
        }
        //
        std::unique_ptr<SSGroupMembersT[]> data(new SSGroupMembersT[count]);
        int index = 0;
        for (int n = 0; n < getMemberResponse->get_member_list_size(); n++) {
            auto_dispose_ptr<SSMember> members(getMemberResponse->get_member_list(n));

            for (int i = 0; i < members->get_members_size(); i++) {
                strncpy(data[index].groupid, members->get_groupid(), USER_ID_LENGTH);
                strncpy(data[index].contactid, members->get_members(i), USER_ID_LENGTH);
                index++;
            }
        }
        CallbackLog(index, ID_GroupMembersResp, Result_Success, data.get(), count);
    }
    END_ASYNC_CALL2(this, index)
    //=========================================================================
    return Result_Success;
}//批量返回，内部需要结尾

SSResultCode UserInterface::AddGroupMembers(unsigned int index, const ::SSGroupMembersT* members, unsigned int length ) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("AddGroupMembers(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    
    //=========================================================================
    BEGIN_ASYNC_CALL4(UserInterface*, ui, unsigned int, index, SSGroupMembersT*, members, unsigned int, length)
    void ctor() {
        members = reallocate(members, sizeof(SSGroupMembersT)*length);
    }
    void dtor() {
        release(members);
    }
    void call() {
        auto_dispose_ptr<AddMemberRequest> addMemberRequest(AddMemberRequest::New());
        addMemberRequest->set_userid(ui->m_strUserId.c_str());
        for (unsigned int i = 0; i < length; i++) {
            auto_dispose_ptr<SSMember> member(SSMember::New());
            member->set_groupid(members[i].groupid);
            member->add_members(members[i].contactid);
            addMemberRequest->add_member_list(member);
        }
        auto_dispose_ptr<AddMemberResponse> addMemberResponse;
        if (g_pDataDomainSvc->AddMember(addMemberRequest, &addMemberResponse) != 0) {
            SSLOG_ERROR(_T("Can't add member."));
            CallbackLog(index, ID_AddMembersResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(addMemberResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while adding member: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_AddMembersResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_AddMembersResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL4(this, index, (SSGroupMembersT*)members, length)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::RemoveGroupMembers(unsigned int index, const ::SSGroupMembersT* members, unsigned int length ) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("RemoveGroupMembers(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL4(UserInterface*, ui, unsigned int, index, SSGroupMembersT*, members, unsigned int, length)
    void ctor() {
        members = reallocate(members, sizeof(SSGroupMembersT)*length);
    }
    void dtor() {
        release(members);
    }
    void call() {
        auto_dispose_ptr<RemoveMemberRequest> removeMemberRequest(RemoveMemberRequest::New());
        removeMemberRequest->set_userid(ui->m_strUserId.c_str());
        for (unsigned int i = 0; i < length; i++) {
            auto_dispose_ptr<SSMember> member(SSMember::New());
            member->set_groupid(members[i].groupid);
            member->add_members(members[i].contactid);
            removeMemberRequest->add_member_list(member);
        }
        auto_dispose_ptr<RemoveMemberResponse> removeMemberResponse;
        if (g_pDataDomainSvc->RemoveMember(removeMemberRequest, &removeMemberResponse) != 0) {
            SSLOG_ERROR(_T("Can't remove member."));
            CallbackLog(index, ID_RemoveMembersResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(removeMemberResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while removing member: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_AddMembersResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_RemoveMembersResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL4(this, index, (SSGroupMembersT*)members, length)
    //=========================================================================
    return Result_Success;
}


SSResultCode UserInterface::PublicShare(unsigned int index, const ::ShareMsg &shareContent) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("PublicShare(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, ShareMsg*, shareContent)
    void ctor() {
        shareContent = reallocate(shareContent, sizeof(ShareMsg));
        if (shareContent->body != NULL) {
            shareContent->body = reallocate(shareContent->body, shareContent->LengthBody);
        }
        if (shareContent->recievers != NULL) {
            shareContent->recievers = reallocate(shareContent->recievers, shareContent->LengthRecv);
        }
    }
    void dtor() {
        release(shareContent);
    }
    void call() {
        // 设置公开分享序号
        // 2010-10-29，yaosw删除，由客户端设置序号
        //shareContent->seqNum = ::InterlockedIncrement((unsigned int*)&g_last_seq_number);
        //
        auto_dispose_ptr<SendPublicMessageRequest> sendPublicMessageRequest(SendPublicMessageRequest::New());
        auto_dispose_ptr<SSMessage> ssmessage(SSMessage::New());
        ssmessage->set_timestamp(::time(NULL));
        ssmessage->set_subject("");
        ssmessage->set_sender(ui->m_strUserId.c_str());
        ssmessage->set_content_type(CONTENT_TYPE_NORMAL);
        convert(ssmessage, shareContent);
        sendPublicMessageRequest->set_msg(ssmessage);
        auto_dispose_ptr<SendPublicMessageResponse> sendPublicMessageResponse;
        if (g_pDataDomainSvc->SendPublicMessage(sendPublicMessageRequest, &sendPublicMessageResponse) != 0) {
            SSLOG_ERROR(_T("Can't send public message."));
            CallbackLog(index, ID_ShareMsgResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(sendPublicMessageResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while sending public message: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_ShareMsgResp, Result_InternalFail, NULL, 0);
            return;
        }
        char* msgid = (char*)sendPublicMessageResponse->get_msgid();
        CallbackLog(index, ID_ShareMsgResp, Result_Success, msgid, strlen(msgid));
    }
    END_ASYNC_CALL3(this, index, (ShareMsg*)&shareContent)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::ProtectShare(unsigned int index, const ::ShareMsg &shareContent) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("ProtectShare(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, ShareMsg*, shareContent)
    void ctor() {
        shareContent = reallocate(shareContent, sizeof(ShareMsg));
        if (shareContent->body != NULL) {
            shareContent->body = reallocate(shareContent->body, shareContent->LengthBody);
        }
        if (shareContent->recievers != NULL) {
            shareContent->recievers = reallocate(shareContent->recievers, shareContent->LengthRecv);
        }
    }
    void dtor() {
        release(shareContent);
    }
    void call() {
        //CallbackLog(index, ID_ShareMsgResp, Result_FunctionNoImplement, NULL, 0);
        //return;

        auto_dispose_ptr<SendMessageRequest> sendMessageRequest(SendMessageRequest::New());
        auto_dispose_ptr<SSMessage> ssmessage(SSMessage::New());
        ssmessage->set_timestamp(::time(NULL));
        ssmessage->set_subject("");
        ssmessage->set_sender(ui->m_strUserId.c_str());
        ssmessage->set_content_type(CONTENT_TYPE_NORMAL);
        convert(ssmessage, shareContent);
        sendMessageRequest->set_msg(ssmessage);
        auto_dispose_ptr<SendMessageResponse> sendMessageResponse;
        if (g_pDataDomainSvc->SendMessage(sendMessageRequest, &sendMessageResponse) != 0) {
            SSLOG_ERROR(_T("Can't send protected message."));
            CallbackLog(index, ID_ShareMsgResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(sendMessageResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while sending protected message: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_ShareMsgResp, Result_InternalFail, NULL, 0);
            return;
        }
        char* msgid = (char*)sendMessageResponse->get_msgid();
        CallbackLog(index, ID_ShareMsgResp, Result_Success, msgid, strlen(msgid));
    }
    END_ASYNC_CALL3(this, index, (ShareMsg*)&shareContent)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::PrivateShare(unsigned int index, const ::ShareMsg & shareContent) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("PrivateShare(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, ShareMsg*, shareContent)
    void ctor() {
        shareContent = reallocate(shareContent, sizeof(ShareMsg));
        if (shareContent->body != NULL) {
            shareContent->body = reallocate(shareContent->body, shareContent->LengthBody);
        }
        if (shareContent->recievers != NULL) {
            shareContent->recievers = reallocate(shareContent->recievers, shareContent->LengthRecv);
        }
    }
    void dtor() {
        release(shareContent);
    }
    void call() {
        //CallbackLog(index, ID_ShareMsgResp, Result_FunctionNoImplement, NULL, 0);
        //return;

        auto_dispose_ptr<AddFavoriteMessageRequest> addFavoriteMessageRequest(AddFavoriteMessageRequest::New());
        auto_dispose_ptr<SSMessage> ssmessage(SSMessage::New());
        ssmessage->set_timestamp(::time(NULL));
        ssmessage->set_subject("");
        ssmessage->set_sender(ui->m_strUserId.c_str());
        ssmessage->set_content_type(CONTENT_TYPE_NORMAL);
        convert(ssmessage, shareContent);
        addFavoriteMessageRequest->set_msg(ssmessage);
        auto_dispose_ptr<AddFavoriteMessageResponse> addFavoriteMessageResponse;
        if (g_pDataDomainSvc->AddFavoriteMessage(addFavoriteMessageRequest, &addFavoriteMessageResponse) != 0) {
            SSLOG_ERROR(_T("Can't send private message."));
            CallbackLog(index, ID_ShareMsgResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(addFavoriteMessageResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while sending private message: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_ShareMsgResp, Result_InternalFail, NULL, 0);
            return;
        }
        char* msgid = (char*)addFavoriteMessageResponse->get_msgid();
        CallbackLog(index, ID_ShareMsgResp, Result_Success, msgid, strlen(msgid));
    }
    END_ASYNC_CALL3(this, index, (ShareMsg*)&shareContent)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetMsgFromInBox(unsigned int index, const char* msgid, int      getCount) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetMsgFromInBox(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL4(UserInterface*, ui, unsigned int, index, char*, msgid, int, count)
    void ctor() {
        msgid = reallocate(msgid, MSG_ID_SIZE);
    }
    void dtor() {
        release(msgid);
    }
    void call() {
        auto_dispose_ptr<GetMessageRequest> getMessageRequest(GetMessageRequest::New());
        getMessageRequest->set_userid(ui->m_strUserId.c_str());
        getMessageRequest->set_source_box(INBOX);
        if (strlen(msgid) > 0) {
            getMessageRequest->set_cursor_id(msgid);
        }
        if (count != 0) {
            getMessageRequest->set_count(count);
        }
        auto_dispose_ptr<GetMessageResponse> getMessageResponse;
        if (g_pDataDomainSvc->GetMessage(getMessageRequest, &getMessageResponse) != 0) {
            SSLOG_ERROR(_T("Can't get message from inbox."));
            CallbackLog(index, ID_GetInBoxResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getMessageResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting message from inbox: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_GetInBoxResp, Result_InternalFail, NULL, 0);
            return;
        }
        int count = getMessageResponse->get_msg_list_size();
        std::unique_ptr<ShareMsg[]> data(new ShareMsg[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSMessage> message(getMessageResponse->get_msg_list(i));
            convert(&data[i], message, PROTECT_SHARE);
        }
        for (int i = 0; i < count; i++) {
            CallbackLog(index, ID_GetInBoxResp, Result_Success, &data[i], 1);
        }
    }
    END_ASYNC_CALL4(this, index, (char*)msgid, getCount)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetProtectShareMsgFromOutBox(unsigned int index, const char* msgid,
                                                      int      getCount) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetProtectShareMsgFromOutBox(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL4(UserInterface*, ui, unsigned int, index, char*, msgid, int, count)
    void ctor() {
        msgid = reallocate(msgid, MSG_ID_SIZE);
    }
    void dtor() {
        release(msgid);
    }
    void call() {
        auto_dispose_ptr<GetMessageRequest> getMessageRequest(GetMessageRequest::New());
        getMessageRequest->set_userid(ui->m_strUserId.c_str());
        getMessageRequest->set_source_box(OUTBOX);
        if (strlen(msgid) > 0) {
            getMessageRequest->set_cursor_id(msgid);
        }
        if (count != 0) {
            getMessageRequest->set_count(count);
        }
        auto_dispose_ptr<GetMessageResponse> getMessageResponse;
        if (g_pDataDomainSvc->GetMessage(getMessageRequest, &getMessageResponse) != 0) {
            SSLOG_ERROR(_T("Can't get message from outbox."));
            CallbackLog(index, ID_GetProtectShareOutBoxResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getMessageResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting message from outbox: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_GetProtectShareOutBoxResp, Result_InternalFail, NULL, 0);
            return;
        }
        int count = getMessageResponse->get_msg_list_size();
        std::unique_ptr<ShareMsg[]> data(new ShareMsg[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSMessage> message(getMessageResponse->get_msg_list(i));
            convert(&data[i], message, PROTECT_SHARE);
        }
        for (int i = 0; i < count; i++) {
            CallbackLog(index, ID_GetProtectShareOutBoxResp, Result_Success, &data[i], 1);
        }
    }
    END_ASYNC_CALL4(this, index, (char*)msgid, getCount)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetPublicShareMsgFromOutBox(unsigned int index,  const char* msgid,
                                                      int    getCount) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetPublicShareMsgFromOutBox(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL4(UserInterface*, ui, unsigned int, index, char*, msgid, int, count)
    void ctor() {
        msgid = reallocate(msgid, MSG_ID_SIZE);
    }
    void dtor() {
        release(msgid);
    }
    void call() {
        auto_dispose_ptr<GetMessageRequest> getMessageRequest(GetMessageRequest::New());
        getMessageRequest->set_userid(ui->m_strUserId.c_str());
        getMessageRequest->set_source_box(PUBBOX);
        if (strlen(msgid) > 0) {
            getMessageRequest->set_cursor_id(msgid);
        }
        if (count != 0) {
            getMessageRequest->set_count(count);
        }
        auto_dispose_ptr<GetMessageResponse> getMessageResponse;
        if (g_pDataDomainSvc->GetMessage(getMessageRequest, &getMessageResponse) != 0) {
            SSLOG_ERROR(_T("Can't get public message from outbox."));
            CallbackLog(index, ID_GetPublicShareOutBoxResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getMessageResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting public message from outbox: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_GetPublicShareOutBoxResp, Result_InternalFail, NULL, 0);
            return;
        }
        int count = getMessageResponse->get_msg_list_size();
        std::unique_ptr<ShareMsg[]> data(new ShareMsg[count]);
        for (int i = 0; i < getMessageResponse->get_msg_list_size(); i++) {
            auto_dispose_ptr<SSMessage> message(getMessageResponse->get_msg_list(i));
            convert(&data[i], message, PUBLIC_SHARE);
        }
        for (int i = 0; i < count; i++) {
            CallbackLog(index, ID_GetPublicShareOutBoxResp, Result_Success, &data[i], 1);
        }
    }
    END_ASYNC_CALL4(this, index, (char*)msgid, getCount)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetOtherUserPublicShareMsg(unsigned int index, const char* otherUserID,  
                                                    int       seqNumCursor,
                                                    int    getCount) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetOtherUserPublicShareMsg(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL5(UserInterface*, ui, unsigned int, index, char*, otherUserID, int, seq, int, count)
    void ctor() {
        otherUserID = reallocate(otherUserID, USER_ID_SIZE);
    }
    void dtor() {
        release(otherUserID);
    }
    void call() {
        auto_dispose_ptr<GetMessageByTimestampRequest> getMessageByTimestampRequest(GetMessageByTimestampRequest::New());
        getMessageByTimestampRequest->set_userid(otherUserID);
        getMessageByTimestampRequest->set_source_box(PUBBOX);
        getMessageByTimestampRequest->set_start_timestamp(seq);
        getMessageByTimestampRequest->set_end_timestamp(seq + 100);
        auto_dispose_ptr<GetMessageByTimestampResponse> getMessageByTimestampResponse;
        if (g_pDataDomainSvc->GetMessageByTimestamp(getMessageByTimestampRequest, &getMessageByTimestampResponse) != 0) {
            SSLOG_ERROR(_T("Can't get other public message."));
            CallbackLog(index, ID_GetOtherPublicShareResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getMessageByTimestampResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting public message from outbox: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_GetOtherPublicShareResp, Result_InternalFail, NULL, 0);
            return;
        }
        int count = getMessageByTimestampResponse->get_msg_list_size();
        std::unique_ptr<ShareMsg[]> data(new ShareMsg[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSMessage> message(getMessageByTimestampResponse->get_msg_list(i));
            convert(&data[i], message, PUBLIC_SHARE);
        }
        for (int i = 0; i < count; i++) {
            CallbackLog(index, ID_GetOtherPublicShareResp, Result_Success, &data[i], 1);
        }
    }
    END_ASYNC_CALL5(this, index, (char*)otherUserID, seqNumCursor, getCount)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::FollowUser(unsigned int index,  const char* users) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("FollowUser(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, char*, users)
    void ctor() {
        users = reallocate(users, strlen(users)+1);
    }
    void dtor() {
        release(users);
    }
    void call() {
        StringSplit split(users, ",");
        auto_dispose_ptr<FollowRequest> followRequest(FollowRequest::New());
        followRequest->set_follower_userid(ui->m_strUserId.c_str());
        while (!split.eof()) {
            followRequest->add_following_userid_list(split.next());
        }
        auto_dispose_ptr<FollowResponse> followResponse;
        if (g_pDataDomainSvc->Follow(followRequest, &followResponse) != 0) {
            SSLOG_ERROR(_T("Can't follow user."));
            CallbackLog(index, ID_FollowUserResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(followResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while following: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_FollowUserResp, Result_InternalFail, NULL, 0);
            return;
        }
        g_message_pump_station->notify_following_changed();
        CallbackLog(index, ID_FollowUserResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (char*)users)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::UnFollowUser(unsigned int index, const  char* users) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("UnFollowUser(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, char*, users)
    void ctor() {
        users = reallocate(users, strlen(users)+1);
    }
    void dtor() {
        release(users);
    }
    void call() {
        StringSplit split(users, ",");
        auto_dispose_ptr<CancelFollowRequest> cancelFollowRequest(CancelFollowRequest::New());
        cancelFollowRequest->set_follower_userid(ui->m_strUserId.c_str());
        while (!split.eof()) {
            cancelFollowRequest->add_following_userid_list(split.next());
        }
        auto_dispose_ptr<CancelFollowResponse> cancelFollowResponse;
        if (g_pDataDomainSvc->CancelFollow(cancelFollowRequest, &cancelFollowResponse) != 0) {
            SSLOG_ERROR(_T("Can't cancel follow user."));
            CallbackLog(index, ID_UnFollowUserResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(cancelFollowResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while canceling follow user: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_UnFollowUserResp, Result_InternalFail, NULL, 0);
            return;
        }
        g_message_pump_station->notify_following_changed();
        CallbackLog(index, ID_UnFollowUserResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (char*)users)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::Invite(unsigned int index, const char* users)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("Invite(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL3(UserInterface*, ui, unsigned int, index, char*, users)
    void ctor() {
        users = reallocate(users, strlen(users)+1);
    }
    void dtor() {
        release(users);
    }
    void call() {
        StringSplit split(users, ",");
        auto_dispose_ptr<InviteRequest> inviteRequest(InviteRequest::New());
        inviteRequest->set_inviter_userid(ui->m_strUserId.c_str());
        while (!split.eof()) {
            inviteRequest->add_inviting_userid_list(split.next());
        }
        auto_dispose_ptr<InviteResponse> inviteResponse;
        if (g_pDataDomainSvc->Invite(inviteRequest, &inviteResponse) != 0) {
            SSLOG_ERROR(_T("Can't save invite record."));
            CallbackLog(index, ID_InviteResp, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(inviteResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while saving invite record: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_InviteResp, Result_InternalFail, NULL, 0);
            return;
        }
        CallbackLog(index, ID_InviteResp, Result_Success, NULL, 0);
    }
    END_ASYNC_CALL3(this, index, (char*)users)
    //=========================================================================
    return Result_Success;
}

SSResultCode UserInterface::GetInviterList(unsigned int index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    SSLOG_DEBUG(_T("GetInviterList(") << index << _T(", ...)"));
    ENSURE_INITLIALIZATION();
    ENSURE_LOGIN();
    
    //=========================================================================
    BEGIN_ASYNC_CALL2(UserInterface*, ui, unsigned int, index)
    void ctor() {
    }
    void dtor() {
    }
    void call() {
        auto_dispose_ptr<GetInviterWithProfileRequest> getInviterWithProfileRequest(GetInviterWithProfileRequest::New());
        getInviterWithProfileRequest->set_userid(ui->m_strUserId.c_str());
        getInviterWithProfileRequest->set_exclude_icon(false);
        auto_dispose_ptr<GetInviterWithProfileResponse> getInviterWithProfileResponse;
        if (g_pDataDomainSvc->GetInviterWithProfile(getInviterWithProfileRequest, &getInviterWithProfileResponse) != 0) {
            SSLOG_ERROR(_T("Can't get inviter list."));
            CallbackLog(index, ID_InviterListMsg, Result_ConnectFail, NULL, 0);
            return;
        }
        auto_dispose_ptr<ServiceResult> result(getInviterWithProfileResponse->get_result());
        if (result->get_code() != SC_OK) {
            SSLOG_ERROR(_T("Error occurs while getting inviter list: ")
                << result->get_code()
                << _T(",")
                << UTF8TOTCHAR(result->get_message()));
            CallbackLog(index, ID_InviterListMsg, Result_InternalFail, NULL, 0);
            return;
        }
        // 转换profile
        int count = getInviterWithProfileResponse->get_inviter_list_size();
        std::unique_ptr<SSProfileT[]> data(new SSProfileT[count]);
        for (int i = 0; i < count; i++) {
            auto_dispose_ptr<SSProfile> profile(getInviterWithProfileResponse->get_inviter_list(i));
            convert(&data[i], profile);
        }
        CallbackLog(index, ID_InviterListMsg, Result_Success, data.get(), count);
    }
    END_ASYNC_CALL2(this, index)
    //=========================================================================
    return Result_Success;
}
