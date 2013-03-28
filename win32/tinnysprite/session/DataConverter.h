/*--------------------------------------------------------------------------------*\

     文件：  DataConverter.h
     作者：  姚松文
     日期：  2010年10月20日
     说明：  接口上的数据转换

\*--------------------------------------------------------------------------------*/

#ifndef DATA_CONVERTER_H
#define DATA_CONVERTER_H

#define __SD_CLIENT_DLL_INTERNAL_USE__
#include <ISessionDomainClient.h>

#include "SSDataDomain.api.h"

void convert(SSProfileT* profile_t, SSDataDomainClient::SSProfile* profile);
void convert(SSDataDomainClient::SSProfile* profile, SSProfileT* profile_t);

void convert(ShareMsg* message_t, SSDataDomainClient::SSMessage* message, ShareType share_type/*, SSMessageType msg_type*/);
void convert(SSDataDomainClient::SSMessage* message, ShareMsg* message_t);

void convert(SSContactsT* contact_t, SSDataDomainClient::SSContactEx* contact_ex);
//void convert(SSDataDomainClient::SSContactEx* contact_ex, SSContactsT* contact_t);
void convert(SSDataDomainClient::SSContact* contact, SSContactsExT* contact_ex_t);

void convert(SSGroupT* group_t, SSDataDomainClient::SSGroup* group);
void convert(SSDataDomainClient::SSGroup* group, SSGroupT* group_t);

//void convert(SSGroupMembersT* member_t, SSDataDomainClient::SSMember* member);
//void convert(SSDataDomainClient::SSMember* member, SSGroupMembersT* member_t);

void convert(SSLastTimeT* client_state_t, SSDataDomainClient::SSClientState* client_state);
void convert(SSDataDomainClient::SSClientState* client_state, SSLastTimeT* client_state_t);
#endif // DATA_CONVERTER_H