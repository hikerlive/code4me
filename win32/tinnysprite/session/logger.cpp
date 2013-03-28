/*--------------------------------------------------------------------------------*\

     文件：  logger.cpp
     作者：  姚松文
     日期：  2010年10月20日
     说明：  无

\*--------------------------------------------------------------------------------*/

#include "stdafx.h"

#ifndef DISABLE_LOGGER
#include <tchar.h>

#define __SD_CLIENT_DLL_INTERNAL_USE__
// #include <SSSessionDomainClient\ISessionDomainClient.h>

#include "logger.h"
#pragma comment(lib, "log4cxx")

void initialize_logger() {
    static bool initialized = false;

    if (initialized)
        return;
    
    initialized = true;
    
    TCHAR szBaseDir[MAX_PATH];
    GetModuleFileName(NULL, szBaseDir, MAX_PATH);
    TCHAR * p1 = szBaseDir;
    TCHAR * p2 = NULL;
    while (p1 != NULL) {
        p2 = _tcsstr(p1, _T("\\"));
        if (p2 == NULL) {
            if (p1 != szBaseDir) {
                *p1 = _T('\0');
            }
            break;
        }
        p1 = p2 + 1;
    }
    while (szBaseDir[_tcslen(szBaseDir)-1] == _T('\\'))
        szBaseDir[_tcslen(szBaseDir)-1] = _T('\0');

    TCHAR szConfigFilePath[MAX_PATH];
    _tcscpy(szConfigFilePath, szBaseDir);
    _tcscat(szConfigFilePath, _T("\\log4cxx.properties"));

    log4cxx::helpers::InputStreamPtr is = new log4cxx::helpers::FileInputStream(szConfigFilePath);
    log4cxx::helpers::Properties props;
    props.load(is);
    props.setProperty(_T("app.base"), szBaseDir);

    TCHAR szUserDir[MAX_PATH];
    GetEnvironmentVariable(_T("USERPROFILE"), szUserDir, MAX_PATH);
    props.setProperty(_T("sys.user.home"), szUserDir);

    log4cxx::PropertyConfigurator::configure(props);
}

LogStream& log(LogStream& oss, CallBackCMDID id) {
    switch (id) {
    case ID_RegistResp:
        oss << _T("ID_RegistResp");
        break;
    case ID_LoginResp:
        oss << _T("ID_LoginResp");
        break;
    case ID_ProfileResp:
        oss << _T("ID_ProfileResp");
        break;
    case ID_ContactsResp:
        oss << _T("ID_ContactsResp");
        break;
    case ID_GroupResp:
        oss << _T("ID_GroupResp");
        break;
    case ID_GroupMembersResp:
        oss << _T("ID_GroupMembersResp");
        break;
    case ID_IconResp:
        oss << _T("ID_IconResp");
        break;
    case ID_SetContactResp:
        oss << _T("ID_SetContactResp");
        break;
    case ID_AddContactsResp:
        oss << _T("ID_AddContactsResp");
        break;
    case ID_RemoveContactsResp:
        oss << _T("ID_RemoveContactsResp");
        break;
    case ID_SetGroupResp:
        oss << _T("ID_SetGroupResp");
        break;
    case ID_AddGroupResp:
        oss << _T("ID_AddGroupResp");
        break;
    case ID_RemoveGroupResp:
        oss << _T("ID_RemoveGroupResp");
        break;
    case ID_AddMembersResp:
        oss << _T("ID_AddMembersResp");
        break;
    case ID_RemoveMembersResp:
        oss << _T("ID_RemoveMembersResp");
        break;
    case ID_FollowCountResp:
        oss << _T("ID_FollowCountResp");
        break;
    case ID_FollowListResp:
        oss << _T("ID_FollowListResp");
        break;
    case ID_FollowingCountResp:
        oss << _T("ID_FollowingCountResp");
        break;
    case ID_FollowingListResp:
        oss << _T("ID_FollowingListResp");
        break;
    case ID_OtherFollowCountResp:
        oss << _T("ID_OtherFollowCountResp");
        break;
    case ID_OtherFollowListResp:
        oss << _T("ID_OtherFollowListResp");
        break;
    case ID_OtherFollowingCountResp:
        oss << _T("ID_OtherFollowingCountResp");
        break;
    case ID_OtherFollowingListResp:
        oss << _T("ID_OtherFollowingListResp");
        break;
    case ID_GetLastTimeResp:
        oss << _T("ID_GetLastTimeResp");
        break;
    case ID_SetLastTimeResp:
        oss << _T("ID_SetLastTimeResp");
        break;
    case ID_OtherUserProfileResp:
        oss << _T("ID_OtherUserProfileResp");
        break;
    case ID_LastPubShareSeqNumMsg:
        oss << _T("ID_LastPubShareSeqNumMsg");
        break;
    case ID_GetPublicShareOutBoxResp:
        oss << _T("ID_GetPublicShareOutBoxResp");
        break;
    case ID_GetProtectShareOutBoxResp:
        oss << _T("ID_GetProtectShareOutBoxResp");
        break;
    case ID_GetPrivateBoxResp:
        oss << _T("ID_GetPrivateBoxResp");
        break;
    case ID_GetInBoxResp:
        oss << _T("ID_GetInBoxResp");
        break;
    case ID_GetOtherPublicShareResp:
        oss << _T("ID_GetOtherPublicShareResp");
        break;
    case ID_OnShareMsg:
        oss << _T("ID_OnShareMsg");
        break;
    case ID_ForgetPwdResp:
        oss << _T("ID_ForgetPwdResp");
        break;
    case ID_VerifyActiveCodeResp:
        oss << _T("ID_VerifyActiveCodeResp");
        break;
    case ID_ChangePwdResp:
        oss << _T("ID_ChangePwdResp");
        break;
    case ID_UserExistResp:
        oss << _T("ID_UserExistResp");
        break;
    case ID_FollowUserResp:
        oss << _T("ID_FollowUserResp");
        break;
    case ID_UnFollowUserResp:
        oss << _T("ID_UnFollowUserResp");
        break;
    case ID_ShareMsgResp:
        oss << _T("ID_ShareMsgResp");
        break;
    case ID_OnLogOutMsg:
        oss << _T("ID_OnLogOutMsg");
        break;
    case ID_InitResult:
        oss << _T("ID_InitResult");
        break;
    case ID_ResetPWD:
        oss << _T("ID_ResetPWD");
        break;
    case ID_NULL:
        oss << _T("ID_NULL");
        break;
    default:
        oss << _T("Unknown CallBackCMDID");
        break;
    }

    return oss;
}

LogStream& log(LogStream& oss, SSResultCode result)
{
    switch (result) {
    case Result_Success:
        oss << _T("Result_Success");
        break;
	case Result_ParameterWrong:
        oss << _T("Result_ParameterWrong");
        break;
	case Result_FunctionNoImplement:
        oss << _T("Result_FunctionNoImplement");
        break;
	case Result_InternalFail:
        oss << _T("Result_InternalFail");
        break;

	case Result_SendFail:
        oss << _T("Result_SendFail");
        break;
	case Result_SendTimeOut:
        oss << _T("Result_SendTimeOut");
        break;
	case Result_ReceiveFail:
        oss << _T("Result_ReceiveFail");
        break;
	case Result_ReceiveTimeOut:
        oss << _T("Result_ReceiveTimeOut");
        break;
	case Result_ConnectFail:
        oss << _T("Result_ConnectFail");
        break;

	case Result_InitFail:
        oss << _T("Result_InitFail");
        break;
	case Result_UserIsRegisted:
        oss << _T("Result_UserIsRegisted");
        break;
	case Result_UserNotExist:
        oss << _T("Result_UserNotExist");
        break;
	case Result_PasswordError:
        oss << _T("Result_PasswordError");
        break;
	case Result_SendEmailError:
        oss << _T("Result_SendEmailError");
        break;
	case Result_VerifyActiveCode:
        oss << _T("Result_VerifyActiveCode");
        break;
	case Result_VerifyActiveCodeTimeOut:
        oss << _T("Result_VerifyActiveCodeTimeOut");
        break;
	case Result_HaveFollowed:
        oss << _T("Result_HaveFollowed");
        break;
	case Result_HaveNotFollowed:
        oss << _T("Result_HaveNotFollowed");
        break;
    case Result_UserNotLogin:
        oss << _T("Result_UserNotLogin");
        break;
    default:
        oss << _T("Unknown SSResultCode");
        break;
    }
    return oss;
}

LogStream& log(LogStream& oss, ShareType type)
{
    switch(type) {
    case PUBLIC_SHARE:
        oss << _T("PUBLIC_SHARE");
        break;
    case PROTECT_SHARE:
        oss << _T("PROTECT_SHARE");
        break;
    case PRIVATE_SHARE:
        oss << _T("PRIVATE_SHARE");
        break;
    default:
        oss << _T("Unknown ShareType");
        break;
    }
    return oss;
}

LogStream& log(LogStream& oss, SSMessageType type)
{
    switch(type) {
    case SM_NORMAL:
        oss << _T("SM_NORMAL");
        break;
    case SM_PROFILE:
        oss << _T("SM_PROFILE");
        break;
    case SM_ICON:
        oss << _T("SM_ICON");
        break;
    default:
        oss << _T("Unknown SSMessageType");
        break;
    }
    return oss;
}

LogStream& log(LogStream& oss, unsigned char* bytes, int len)
{
    if (bytes == NULL) {
        oss << _T("NULL");
        return oss;
    }

    for (int i = 0; i < len; i++) {
        if (i == 0) {
            oss << _T("0x");
        }

        if (i >= 8) {
            oss << _T("...");
            break;
        }

        TCHAR szBuffer[16];
        memset(szBuffer, 0, sizeof(TCHAR)*16);
        _stprintf(szBuffer, _T("%02X"), bytes[i]);
        oss << szBuffer;
    }

    return oss;
}

LogStream& log(LogStream& oss, SSGroupT* p, int count)
{
    oss << _T("[");
    for (int i = 0; i < count; i++) {
        oss << _T("\n");
        oss << _T("\"SSGroupT\" : {") << _T("\n");
        oss << _T("\t") << _T("\"groupid\": \"") << UTF8TOTCHAR(p[i].groupid) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"name\": \"") << UTF8TOTCHAR(p[i].name) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"proprety\": ") << p[i].proprety << _T(", ") << _T("\n");
        oss << _T("}, ");
    }
    oss << _T("]");

    return oss;
}

LogStream& log(LogStream& oss, SSContactsT* p, int count)
{
    oss << _T("[");
    for (int i = 0; i < count; i++) {
        oss << _T("\n");
        oss << _T("\"SSContactsT\" : {") << _T("\n");
        oss << _T("\t") << _T("\"contactid\": \"") << UTF8TOTCHAR(p[i].contactid) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"user\": \"") << UTF8TOTCHAR(p[i].user) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"userid\": \"") << UTF8TOTCHAR(p[i].userid) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"nickname\": \"") << UTF8TOTCHAR(p[i].nickname) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"nickalias\": \"") << UTF8TOTCHAR(p[i].nickalias) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"age\": ") << p[i].age << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"sex\": ") << p[i].sex << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"IsFollower\": ") << p[i].IsFollower << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"IsFollowing\": ") << p[i].IsFollowing << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"IsInvite\": ") << p[i].IsInvite << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"Issystem\": ") << p[i].Issystem << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"iconname\": \"") << UTF8TOTCHAR(p[i].iconname) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"remark\": \"") << UTF8TOTCHAR(p[i].remark) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"LengthEx\": ") << p[i].LengthEx << _T(", ") << _T("\n");
        oss << _T("}, ");
    }
    oss << _T("]");

    return oss;
}

LogStream& log(LogStream& oss, SSIconT* p, int count)
{
    oss << _T("[");
    for (int i = 0; i < count; i++) {
        oss << _T("\n");
        oss << _T("\"SSIconT\" : {") << _T("\n");
        oss << _T("\t") << _T("\"userid\": \"") << UTF8TOTCHAR(p[i].userid) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"iconname\": \"") << UTF8TOTCHAR(p[i].iconname) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"LengthIcon\": ") << p[i].LengthIcon << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"icon\": \"");log(oss, (unsigned char*)(p[i].icon), p[i].LengthIcon);oss << _T("\", ") << _T("\n");
        oss << _T("}, ");
    }
    oss << _T("]");

    return oss;
}

LogStream& log(LogStream& oss, SSProfileT* p, int count)
{
    oss << _T("[");
    for (int i = 0; i < count; i++) {
        oss << _T("\n");
        oss << _T("\"SSProfileT\" : {") << _T("\n");
        oss << _T("\t") << _T("\"userid\": \"") << UTF8TOTCHAR(p[i].userid) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"nickname\": \"") << UTF8TOTCHAR(p[i].nickname) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"age\": ") << p[i].age << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"sex\": ") << p[i].sex << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"remark\": \"") << UTF8TOTCHAR(p[i].remark) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"iconname\": \"") << UTF8TOTCHAR(p[i].iconname) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"LengthIcon\": ") << p[i].LengthIcon << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"icon\": \"");log(oss, (unsigned char*)(p[i].icon), p[i].LengthIcon);oss << _T("\"") << _T("\n");
        oss << _T("}, ");
    }
    oss << _T("]");

    return oss;
}

LogStream& log(LogStream& oss, SSLastTimeT* p, int count)
{
    oss << _T("[");
    for (int i = 0; i < count; i++) {
        oss << _T("\n");
        oss << _T("\"SSLastTimeT\" : {") << _T("\n");
        oss << _T("\t") << _T("\"last_contact_time\": \"") << UTF8TOTCHAR(p[i].last_contact_time) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"last_favo_time\": \"") << UTF8TOTCHAR(p[i].last_favo_time) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"last_follow_time\": \"") << UTF8TOTCHAR(p[i].last_follow_time) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"last_group_time\": \"") << UTF8TOTCHAR(p[i].last_group_time) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"last_member_time\": \"") << UTF8TOTCHAR(p[i].last_member_time) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"last_recv_time\": \"") << UTF8TOTCHAR(p[i].last_recv_time) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"last_send_time\": \"") << UTF8TOTCHAR(p[i].last_send_time) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"last_user_time\": \"") << UTF8TOTCHAR(p[i].last_user_time) << _T("\", ") << _T("\n");
        oss << _T("}, ");
    }
    oss << _T("]");

    return oss;
}

LogStream& log(LogStream& oss, SSUserExistT* p, int count)
{
    oss << _T("[");
    for (int i = 0; i < count; i++) {
        oss << _T("\n");
        oss << _T("\"SSUserExistT\" : {") << _T("\n");
        oss << _T("\t") << _T("\"userId\": \"") << UTF8TOTCHAR(p[i].userId) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"isSystem\": ") << p[i].isSystem << _T(", ") << _T("\n");
        oss << _T("}, ");
    }
    oss << _T("]");

    return oss;
}

LogStream& log(LogStream& oss, SSGroupMembersT* p, int count)
{
    oss << _T("[");
    for (int i = 0; i < count; i++) {
        oss << _T("\n");
        oss << _T("\"SSGroupMembersT\" : {") << _T("\n");
        oss << _T("\t") << _T("\"groupid\": \"") << UTF8TOTCHAR(p[i].groupid) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"contactid\": \"") << UTF8TOTCHAR(p[i].contactid) << _T("\", ") << _T("\n");
        oss << _T("}, ");
    }
    oss << _T("]");

    return oss;
}

LogStream& log(LogStream& oss, ShareMsg* p, int count)
{
    oss << _T("[");
    for (int i = 0; i < count; i++) {
        oss << _T("\n");
        oss << _T("\"ShareMsg\" : {");
        oss << _T("\t") << _T("\"msgid\": \"") << UTF8TOTCHAR(p[i].msgid) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"parentMsgid\": \"") << UTF8TOTCHAR(p[i].parentMsgid) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"rootMsgid\": \"") << UTF8TOTCHAR(p[i].rootMsgid) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"type\": \""); log(oss, p[i].type) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"seqNum\": ") << p[i].seqNum << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"timestamp\": ") << p[i].timestamp << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"sender\": \"") << UTF8TOTCHAR(p[i].sender) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"recievers\": \"") << (p[i].recievers == NULL ? _T("NULL") : UTF8TOTCHAR(p[i].recievers)) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"subject\": \"") << UTF8TOTCHAR(p[i].subject) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"Msgtype\": \""); log(oss, p[i].Msgtype) << _T("\", ") << _T("\n");
        oss << _T("\t") << _T("\"LengthBody\": ") << p[i].LengthBody << _T(", ") << _T("\n");
        oss << _T("\t") << _T("\"body\": \""); log(oss, (unsigned char*)p[i].body, p[i].LengthBody) << _T("\", ") << _T("\n");
        oss << _T("}, ");
    }
    oss << _T("]");

    return oss;
}
#endif // DISABLE_LOGGER