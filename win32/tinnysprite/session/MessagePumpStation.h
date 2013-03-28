/*--------------------------------------------------------------------------------*\

     文件：  MessagePumpStation.h
     作者：  姚松文
     日期：  2010年10月20日
     说明：  消息泵

     用户登录后启动消息泵，当用户收到新消息或用户的关注对象发布公开分享消息后，消息泵将
     消息抽取到客户端，然后通知用户。

\*--------------------------------------------------------------------------------*/

#ifndef MESSAGE_PUMP_STATION_H
#define MESSAGE_PUMP_STATION_H

#define __SD_CLIENT_DLL_INTERNAL_USE__
#include <ISessionDomainClient.h>

#include <string>

#include "SSDataDomain.api.h"

class MessagePumpStation {
public:
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;

    virtual void set_userid(std::string userid) = 0;
    virtual void set_call_back(OnSessionDomainCallBack pCallback) = 0;
    virtual void set_data_svc(SSDataDomainClient::SSDataDomainSvc* pDataSvc) = 0;

    virtual void notify_following_changed() = 0; // 通知消息泵，用户的关注列表有更新。
    //void add_following(std::string following);
    //void remove_following(std::string following);
};

extern MessagePumpStation* g_message_pump_station;

#endif //MESSAGE_PUMP_STATION_H