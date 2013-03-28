/*--------------------------------------------------------------------------------*\

     文件：  MessagePumpStation.cpp
     作者：  姚松文
     日期：  2010年10月20日
     说明：  消息泵

     用户登录后启动消息泵，当用户收到新消息或用户的关注对象发布公开分享消息后，消息泵将
     消息抽取到客户端，然后通知用户。

\*--------------------------------------------------------------------------------*/


#include "stdafx.h"

#include "auto_dispose_ptr.h"

#include <list>

#include <SSConcurrency.h>
#include "MessagePumpStation.h"

#include "DataConverter.h"
DEFINE_LOGGER(_T("stub.message"));

class MessagePumpStationImpl : public MessagePumpStation {
public:
    void start();
    void pause();
    void resume();
    void stop();

    void set_userid(std::string userid);
    void set_call_back(OnSessionDomainCallBack pCallback);
    void set_data_svc(SSDataDomainClient::SSDataDomainSvc* pDataSvc);

    void notify_following_changed();

private:
    enum {
        RUN     = 0,
        PAUSE   = 1,
        STOP    = 2
    };
    void run();
    static void pump_thread(MessagePumpStationImpl* station);
    HANDLE thread;
    int do_stop;
    HANDLE evtSleep;

    SSDataDomainClient::SSDataDomainSvc* pDataSvc;
    OnSessionDomainCallBack pCallback;

    std::string userid;
    std::string last_recieved_message_id;

    std::list<std::pair<std::string, int>> following_list;
    SSDataDomain::SSLock following_lock;
    bool following_changed;
    void refresh_following_list();

    void load_status();
    void save_status();

    void run_once_round();
    std::string fetch_recieved_message(std::string last_recieved_message_id);
    int fetch_following_message(std::string following, int last_sequence);
};

using namespace SSDataDomainClient;
void MessagePumpStationImpl::pump_thread(MessagePumpStationImpl* station) {
    try {
		station->run();
	} catch(...) {
		SSLOG_ERROR(_T("thread [") 
            << GetCurrentThreadId() 
            << _T("] something wrong happened in message pump thread.\n"));
	}
	_endthread();
}

void MessagePumpStationImpl::start() {
    do_stop = RUN;
    evtSleep = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    thread = (HANDLE)_beginthread((void (_cdecl*)(void*))pump_thread, 0, this);

    SSLOG_DEBUG(_T("message pump station is started."));
}

void MessagePumpStationImpl::stop() {
    do_stop = STOP;

    SSLOG_DEBUG(_T("stopping message pump thread."));
    ::SetEvent(this->evtSleep);
    ::WaitForSingleObject(this->thread, INFINITE);
    this->thread = NULL;
    SSLOG_DEBUG(_T("message pump thread is stopped."));

    ::CloseHandle(this->evtSleep);
    this->evtSleep = NULL;
}

void MessagePumpStationImpl::pause() {
    if (do_stop == RUN)
        do_stop = PAUSE;
}

void MessagePumpStationImpl::resume() {
    if (do_stop == PAUSE)
        do_stop = RUN;
}

void MessagePumpStationImpl::run() {
    //// 延迟10秒钟，等待界面启动完成。
    //Sleep(10*1000);

    refresh_following_list();
    load_status();

    while (do_stop != STOP) {
        if (do_stop == PAUSE) {
            //Sleep(1000);
            ::WaitForSingleObject(this->evtSleep, 1000);
            continue;
        }
        
        SSLOG_DEBUG(_T("message pump station is working."));
        try {
            run_once_round();
        } catch(...) {
            SSLOG_ERROR(_T("thread [")
                << GetCurrentThreadId()
                << _T("] something wrong happened in message pump thread.\n"));
        }
        
        SSLOG_DEBUG(_T("message pump station is sleep."));
        // 间隔10秒
        //Sleep(1000*10);
        ::WaitForSingleObject(this->evtSleep, 1000*10);
    }

    SSLOG_DEBUG(_T("message pump thread is exited."));
}

void MessagePumpStationImpl::run_once_round() {
    // 检查关注者列表
    if (following_changed) {
        following_changed = false;
        // 刷新关注者列表
        SSLOG_DEBUG(_T("start refreshing following list."));
        refresh_following_list();
    }

    bool status_changed = false;
    
    // 收取收件箱
    SSLOG_DEBUG(_T("start fetching recieved message."));
    std::string id = fetch_recieved_message(last_recieved_message_id);
    if (id.compare(last_recieved_message_id) != 0) {
        last_recieved_message_id = id;
        status_changed = true;
    }

    // 收取关注者的公开分享
    SSLOG_DEBUG(_T("start fetching following message."));
    std::list<std::pair<std::string,int>>::iterator it = following_list.begin();
    while(it != following_list.end()) {
        int id = fetch_following_message(it->first, it->second);
        if (id != it->second) {
            it->second = id;
            status_changed = true;
        }
        it++;
    }

    // 保存状态到数据域
    if (status_changed) {
        save_status();
    }
}

void MessagePumpStationImpl::load_status() {
    auto_dispose_ptr<GetSessionInfoRequest> getSessionInfoRequest(GetSessionInfoRequest::New());
    getSessionInfoRequest->set_userid(this->userid.c_str());
    auto_dispose_ptr<GetSessionInfoResponse> getSessionInfoResponse;
    if (pDataSvc->GetSessionInfo(getSessionInfoRequest, &getSessionInfoResponse) != 0) {
        SSLOG_ERROR(_T("Can't load status."));
        return;
    }
    auto_dispose_ptr<ServiceResult> result(getSessionInfoResponse->get_result());
    if (result->get_code() != SC_OK) {
        SSLOG_ERROR(_T("Error occur while load status: ")
            << result->get_code()
            << _T(",")
            << UTF8TOTCHAR(result->get_message()));
        return;
    }
    if (!getSessionInfoResponse->has_session_info()) {
        SSLOG_ERROR(_T("No status to load."));
        return;
    }
    auto_dispose_ptr<SSSessionInfo> session_info(getSessionInfoResponse->get_session_info());
    if (session_info->has_client_state()) {
        auto_dispose_ptr<SSClientState> client_state(session_info->get_client_state());
        if (client_state->has_last_recieved_message())
            this->last_recieved_message_id = client_state->get_last_recieved_message();
    }
    for (int i = 0; i < session_info->get_message_state_size(); i++) {
        auto_dispose_ptr<SSMessageState> message_state(session_info->get_message_state(i));

        std::list<std::pair<std::string, int>>::iterator it = following_list.begin();
        while(it != following_list.end()) {
            if (it->first.compare(message_state->get_userid()) == 0) {
                it->second = max(message_state->get_seq_number(), it->second);
                break;
            }
            it++;
        }
    }
}

void MessagePumpStationImpl::save_status() {
    auto_dispose_ptr<SSSessionInfo> session_info(SSSessionInfo::New());
    session_info->set_userid(this->userid.c_str());

    auto_dispose_ptr<SSClientState> client_state(SSClientState::New());
    client_state->set_last_recieved_message(this->last_recieved_message_id.c_str());
    session_info->set_client_state(client_state);

    std::list<std::pair<std::string, int>>::iterator it = following_list.begin();
    while(it != following_list.end()) {
        auto_dispose_ptr<SSMessageState> message_state(SSMessageState::New());
        message_state->set_userid(it->first.c_str());
        message_state->set_seq_number(it->second);
        session_info->add_message_state(message_state);

        it++;
    }

    auto_dispose_ptr<SetSessionInfoRequest> setSessionInfoRequest(SetSessionInfoRequest::New());
    setSessionInfoRequest->set_userid(this->userid.c_str());
    setSessionInfoRequest->set_session_info(session_info);
    auto_dispose_ptr<SetSessionInfoResponse> setSessionInfoResponse;
    if (pDataSvc->SetSessionInfo(setSessionInfoRequest, &setSessionInfoResponse) != 0) {
        SSLOG_ERROR(_T("Can't save status."));
        return;
    }
    auto_dispose_ptr<ServiceResult> result(setSessionInfoResponse->get_result());
    if (result->get_code() != SC_OK) {
        SSLOG_ERROR(_T("Error occur while save status: ")
            << result->get_code()
            << _T(",")
            << UTF8TOTCHAR(result->get_message()));
        return;
    }
}

void MessagePumpStationImpl::refresh_following_list() {
    // 获取关注者列表
    auto_dispose_ptr<GetFollowingRequest> getFollowingRequest(GetFollowingRequest::New());
    getFollowingRequest->set_userid(userid.c_str());
    auto_dispose_ptr<GetFollowingResponse> getFollowingResponse;
    if (pDataSvc->GetFollowing(getFollowingRequest, &getFollowingResponse) != 0) {
        SSLOG_ERROR(_T("Can't refresh following list."));
        return;
    }
    auto_dispose_ptr<ServiceResult> result(getFollowingResponse->get_result());
    if (result->get_code() != SC_OK) {
        SSLOG_ERROR(_T("Error occur while refreshing following list: ")
            << result->get_code()
            << _T(",")
            << UTF8TOTCHAR(result->get_message()));
        return;
    }
    // 合并
    std::list<std::pair<std::string, int>> merged_following_list;
    for (int i = 0; i < getFollowingResponse->get_following_userid_list_size(); i++) {
        std::pair<std::string,int> item(getFollowingResponse->get_following_userid_list(i), 0);
        
        std::list<std::pair<std::string, int>>::iterator it = following_list.begin();
        while(it != following_list.end()) {
            if (it->first.compare(item.first) == 0) {
                item.second = it->second;
                break;
            }
            it++;
        }
        merged_following_list.push_back(item);
    }
    // 替换
    following_list.clear();
    following_list = merged_following_list;
}

std::string MessagePumpStationImpl::fetch_recieved_message(std::string last_recieved_message_id) {
    auto_dispose_ptr<GetMessageRequest> getMessageRequest(GetMessageRequest::New());
    getMessageRequest->set_userid(userid.c_str());
    getMessageRequest->set_source_box(INBOX);
    if (!last_recieved_message_id.empty()) {
        getMessageRequest->set_cursor_id(last_recieved_message_id.c_str());
        getMessageRequest->set_count(-100);
    }
    auto_dispose_ptr<GetMessageResponse> getMessageResponse;
    if (pDataSvc->GetMessage(getMessageRequest, &getMessageResponse) != 0) {
        SSLOG_ERROR(_T("Can't fetch recieved message."));
        return last_recieved_message_id;
    }
    auto_dispose_ptr<ServiceResult> result(getMessageResponse->get_result());
    if (result->get_code() != SC_OK) {
        SSLOG_ERROR(_T("Error occur while fetching recieved message: ")
            << result->get_code()
            << _T(",")
            << UTF8TOTCHAR(result->get_message()));
        return last_recieved_message_id;
    }

    int count = getMessageResponse->get_msg_list_size();
    std::unique_ptr<ShareMsg[]> data(new ShareMsg[count]);
    for (int i = 0; i < count; i++) {
        auto_dispose_ptr<SSMessage> message(getMessageResponse->get_msg_list(i));
        convert(&data[i], message, PROTECT_SHARE);
        if (i == 0) {// 返回列表是倒排的，最前面的是最新的
            last_recieved_message_id = message->get_msgid();
        }
    }
    for (int i = count-1; i >= 0; i--) {
        pCallback(0, ID_OnShareMsg, Result_Success, &data[i], 1);
    }
    return last_recieved_message_id;
}

int MessagePumpStationImpl::fetch_following_message(std::string following, int last_sequence) {
    auto_dispose_ptr<GetMessageByTimestampRequest> getMessageByTimestampRequest(GetMessageByTimestampRequest::New());
    getMessageByTimestampRequest->set_userid(following.c_str());
    getMessageByTimestampRequest->set_source_box(PUBBOX);
    getMessageByTimestampRequest->set_start_timestamp(last_sequence);
    getMessageByTimestampRequest->set_end_timestamp(last_sequence + 100);
    auto_dispose_ptr<GetMessageByTimestampResponse> getMessageByTimestampResponse;
    if (pDataSvc->GetMessageByTimestamp(getMessageByTimestampRequest, &getMessageByTimestampResponse) != 0) {
        SSLOG_ERROR(_T("Can't fetch following [") << following.c_str() << _T("] message."));
        return last_sequence;
    }
    auto_dispose_ptr<ServiceResult> result(getMessageByTimestampResponse->get_result());
    if (result->get_code() != SC_OK) {
        SSLOG_ERROR(_T("Error occur while fetching following [") << following.c_str() << _T("] message: ")
            << result->get_code()
            << _T(",")
            << UTF8TOTCHAR(result->get_message()));
        return last_sequence;
    }

    int count = getMessageByTimestampResponse->get_msg_list_size();
    std::unique_ptr<ShareMsg[]> data(new ShareMsg[count]);
    for (int i = 0; i < count; i++) {
        auto_dispose_ptr<SSMessage> message(getMessageByTimestampResponse->get_msg_list(i));
        convert(&data[i], message, PUBLIC_SHARE);
        
        if (i == 0) {// 返回列表是倒排的，最前面的是最新的
            last_sequence = message->get_seq_number();
        }
    }
    for (int i = count-1; i >= 0; i--) {
        pCallback(0, ID_OnShareMsg, Result_Success, &data[i], 1);
    }
    return last_sequence;
}

void MessagePumpStationImpl::set_call_back(OnSessionDomainCallBack pCallback) {
    this->pCallback = pCallback;
}

void MessagePumpStationImpl::set_userid(std::string userid) {
    this->userid = userid;
}

void MessagePumpStationImpl::set_data_svc(SSDataDomainClient::SSDataDomainSvc* pDataSvc) {
    this->pDataSvc = pDataSvc;
}

void MessagePumpStationImpl::notify_following_changed() {
    following_changed = true;
}

MessagePumpStationImpl instance;
MessagePumpStation* g_message_pump_station = &instance;